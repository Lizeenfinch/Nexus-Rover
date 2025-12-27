#include <SoftwareSerial.h>

//======================================================================
// --- TUNING & CONFIGURATION ---
//======================================================================

// -- Speeds (0-255) --
const int MANUAL_SPEED = 200;       
const int AUTONOMOUS_SPEED = 130;   // Optimized speed

// -- AUTO PILOT TURN CALIBRATION --
const int TURN_90_DURATION = 300;   // Adjust this for exact 90° turn (250-400ms typical)

// -- MANUAL MODE 90° TURN DURATION --
const int MANUAL_TURN_90_DURATION = 300;  // Same as auto pilot for consistency

// -- Voltage Sensor Multiplier --
const float VOLTAGE_SENSOR_MULTIPLIER = 5.0;

// -- Autonomous Behavior --
const int OBSTACLE_DISTANCE_CM = 35;  // Detection distance
const int FOLLOW_DISTANCE_CM = 20;
const int FOLLOW_DEAD_ZONE_CM = 4;

// -- Sensor Logic States --
const int ON_THE_LINE = LOW;
const int ON_THE_SURFACE = HIGH;

// -- Horn Alert Settings --
const int HORN_BEEP_DURATION = 200;

// -- Musical Horn Settings --
const int hornMelody[] = {262, 262, 262, 349, 440, 262, 262, 262, 349, 440};
const int hornDurations[] = {150, 150, 150, 300, 600, 150, 150, 150, 300, 600};
const int melodyLength = 10;

//======================================================================
// --- PIN DEFINITIONS ---
//======================================================================

const int ENA = 11;
const int IN1 = 10;
const int IN2 = 9;
const int ENB = 6;
const int IN3 = 8;
const int IN4 = 7;

const int ESP_RX = 2;
const int ESP_TX = 3;
SoftwareSerial esp8266(ESP_RX, ESP_TX);

const int BUZZER_PIN = 12;
const int SONAR_TRIG_PIN = 4;
const int SONAR_ECHO_PIN = 5;
const int IR_RIGHT_PIN = A2;
const int IR_LEFT_PIN = A1;
const int VOLTAGE_SENSE_PIN = A0;

//======================================================================
// --- GLOBAL VARIABLES ---
//======================================================================

enum DriveMode {
  PARK, LINE_FOLLOW, FOLLOW_ME, EDGE_DETECT, MANUAL, AUTO_PILOT
};
DriveMode currentMode = PARK;

// === ADAPTIVE TURNING VARIABLES ===
bool isMovingForward = false;
bool isMovingBackward = false;

// === TOGGLE MOVEMENT VARIABLES ===
bool forwardToggle = false;  // false = stopped, true = moving
bool reverseToggle = false;  // false = stopped, true = moving

// === MANUAL TURN TIMER ===
unsigned long turnStartTime = 0;
bool isTurning = false;
const int MAX_TURN_DURATION = 1500; // Maximum safety timeout
int previousLeftSpeed = 0;  // Store speeds before turn
int previousRightSpeed = 0;

// === MUSICAL HORN VARIABLES ===
bool hornPlaying = false;
int currentNote = 0;
unsigned long noteStartTime = 0;

// === AUTO PILOT STATE MACHINE ===
enum AutoState {
  AUTO_FORWARD,
  AUTO_REVERSING,
  AUTO_TURNING,
  AUTO_CHECKING
};
AutoState autoState = AUTO_FORWARD;
unsigned long autoStateStartTime = 0;
int turnDirection = 0; // 0 = left, 1 = right

unsigned long lastCommandTime = 0;
const long COMMAND_TIMEOUT = 200;

unsigned long lastTelemetryTime = 0;
const long TELEMETRY_INTERVAL = 1000;

//======================================================================
// --- SETUP ---
//======================================================================

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
  pinMode(IR_RIGHT_PIN, INPUT);
  pinMode(IR_LEFT_PIN, INPUT);

  digitalWrite(BUZZER_PIN, LOW);
  stopMotors();

  lastCommandTime = millis();
  randomSeed(analogRead(A5));
  
  Serial.println("========================================");
  Serial.println("  ROBOT INITIALIZED - 90° TURN MODE");
  Serial.println("  Turn Duration: " + String(TURN_90_DURATION) + "ms");
  Serial.println("========================================");
}

//======================================================================
// --- MAIN LOOP ---
//======================================================================

void loop() {
  if (esp8266.available() > 0) {
    parseCommand();
  }
  
  updateMusicalHorn();
  runCurrentMode();
  sendTelemetry();
}

void runCurrentMode() {
  switch (currentMode) {
    case LINE_FOLLOW: runLineFollower(); break;
    case FOLLOW_ME:   runFollowMe(); break;
    case EDGE_DETECT: runEdgeDetect(); break;
    case AUTO_PILOT:  runAutoPilot(); break;
    case MANUAL:
      // Manual mode with auto-stop for 90° turns
      if (isTurning && (millis() - turnStartTime >= MANUAL_TURN_90_DURATION)) {
        // Auto-stop turn after 90° duration (300ms default)
        isTurning = false;
        
        // Return to previous movement state
        if (forwardToggle) {
          setMotors(MANUAL_SPEED * 0.6, MANUAL_SPEED * 0.6);
          Serial.println("✓ 90° Turn Complete - Resuming forward");
        } else if (reverseToggle) {
          setMotors(-MANUAL_SPEED * 0.6, -MANUAL_SPEED * 0.6);
          Serial.println("✓ 90° Turn Complete - Resuming reverse");
        } else {
          stopMotors();
          Serial.println("✓ 90° Turn Complete - Stopped");
        }
      }
      break;
    case PARK:
      break;
  }
}

//======================================================================
// --- COMMAND & TELEMETRY ---
//======================================================================

void parseCommand() {
  String command = esp8266.readStringUntil('\n');
  command.trim();
  
  if (command.length() == 0) return;

  lastCommandTime = millis();
  Serial.println("Received: " + command);

  char cmd = command.charAt(0);
  
  // Horn commands
  if (cmd == 'H') {
    startMusicalHorn();
    return;
  }
  
  if (cmd == 'h') {
    stopMusicalHorn();
    return;
  }
  
  // Emergency Stop
  if (cmd == 'S') {
    stopMotors();
    isMovingForward = false;
    isMovingBackward = false;
    forwardToggle = false;
    reverseToggle = false;
    isTurning = false;
    Serial.println("🛑 EMERGENCY STOP");
    return;
  }

  // Mode Change
  if (command.startsWith("M")) {
    int modeIndex = command.substring(1).toInt();
    if (modeIndex >= PARK && modeIndex <= AUTO_PILOT) {
      currentMode = (DriveMode)modeIndex;
      stopMotors();
      isMovingForward = false;
      isMovingBackward = false;
      forwardToggle = false;  // Reset toggles
      reverseToggle = false;  // Reset toggles
      isTurning = false;      // Reset turn state
      autoState = AUTO_FORWARD; // Reset auto state
      Serial.println("========================================");
      Serial.println("Mode Changed: " + String(currentMode));
      Serial.println("========================================");
    }
    return;
  }

  // Manual Mode Controls
  if (currentMode == MANUAL) {
    switch (cmd) {
      case 'F':
        // TOGGLE FORWARD - Click once to start, click again to stop
        forwardToggle = !forwardToggle;
        
        if (forwardToggle) {
          // Start moving forward slowly
          setMotors(MANUAL_SPEED * 0.6, MANUAL_SPEED * 0.6); // 60% speed
          isMovingForward = true;
          isMovingBackward = false;
          reverseToggle = false; // Turn off reverse if it was on
          Serial.println("▶️ Forward Started (Slow Speed)");
        } else {
          // Stop
          stopMotors();
          isMovingForward = false;
          Serial.println("⏹ Forward Stopped");
        }
        break;
        
      case 'B':
        // TOGGLE REVERSE - Click once to start, click again to stop
        reverseToggle = !reverseToggle;
        
        if (reverseToggle) {
          // Start reversing slowly
          setMotors(-MANUAL_SPEED * 0.6, -MANUAL_SPEED * 0.6); // 60% speed
          isMovingBackward = true;
          isMovingForward = false;
          forwardToggle = false; // Turn off forward if it was on
          Serial.println("◀️ Reverse Started (Slow Speed)");
        } else {
          // Stop
          stopMotors();
          isMovingBackward = false;
          Serial.println("⏹ Reverse Stopped");
        }
        break;
        
      case 'L':
        // LEFT 90° TURN - Quick pivot turn, auto-completes
        isTurning = true;
        turnStartTime = millis();
        
        // ALWAYS use full power pivot for consistent 90° turn
        // Left wheel backward, right wheel forward
        setMotors(-MANUAL_SPEED, MANUAL_SPEED);
        Serial.println("↺ LEFT 90° Turn - " + String(MANUAL_TURN_90_DURATION) + "ms");
        break;
        
      case 'R':
        // RIGHT 90° TURN - Quick pivot turn, auto-completes
        isTurning = true;
        turnStartTime = millis();
        
        // ALWAYS use full power pivot for consistent 90° turn
        // Left wheel forward, right wheel backward
        setMotors(MANUAL_SPEED, -MANUAL_SPEED);
        Serial.println("↻ RIGHT 90° Turn - " + String(MANUAL_TURN_90_DURATION) + "ms");
        break;
    }
  }
}

void sendTelemetry() {
  if (millis() - lastTelemetryTime > TELEMETRY_INTERVAL) {
    lastTelemetryTime = millis();
    float voltage = analogRead(VOLTAGE_SENSE_PIN) * (5.0 / 1023.0) * VOLTAGE_SENSOR_MULTIPLIER;
    esp8266.println("V:" + String(voltage, 1));
  }
}

//======================================================================
// --- MOTOR CONTROL & UTILITIES ---
//======================================================================

void setMotors(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, leftSpeed < 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed > 0 ? HIGH : LOW);
  analogWrite(ENA, abs(leftSpeed));

  digitalWrite(IN3, rightSpeed < 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed > 0 ? HIGH : LOW);
  analogWrite(ENB, abs(rightSpeed));
}

void stopMotors() {
  setMotors(0, 0);
}

void beepHorn() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(HORN_BEEP_DURATION);
  digitalWrite(BUZZER_PIN, LOW);
}

//======================================================================
// --- MUSICAL HORN FUNCTIONS ---
//======================================================================

void startMusicalHorn() {
  if (!hornPlaying) {
    hornPlaying = true;
    currentNote = 0;
    noteStartTime = millis();
    tone(BUZZER_PIN, hornMelody[currentNote]);
  }
}

void stopMusicalHorn() {
  if (hornPlaying) {
    hornPlaying = false;
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void updateMusicalHorn() {
  if (!hornPlaying) return;
  
  if (millis() - noteStartTime >= hornDurations[currentNote]) {
    currentNote++;
    
    if (currentNote >= melodyLength) {
      stopMusicalHorn();
      return;
    }
    
    noteStartTime = millis();
    tone(BUZZER_PIN, hornMelody[currentNote]);
  }
}

long getSonarDistance() {
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
  long duration = pulseIn(SONAR_ECHO_PIN, HIGH, 25000); 
  return duration * 0.034 / 2;
}

//======================================================================
// --- AUTONOMOUS BEHAVIORS ---
//======================================================================

void runLineFollower() {
  bool leftOnLine = (digitalRead(IR_LEFT_PIN) == ON_THE_LINE);
  bool rightOnLine = (digitalRead(IR_RIGHT_PIN) == ON_THE_LINE);
  
  if (leftOnLine && rightOnLine) {
    beepHorn();
    stopMotors();
  } else if (leftOnLine && !rightOnLine) {
    setMotors(AUTONOMOUS_SPEED / 2, AUTONOMOUS_SPEED);
  } else if (!leftOnLine && rightOnLine) {
    setMotors(AUTONOMOUS_SPEED, AUTONOMOUS_SPEED / 2);
  } else {
    setMotors(AUTONOMOUS_SPEED, AUTONOMOUS_SPEED);
  }
}

void runFollowMe() {
  long distance = getSonarDistance();
  if (distance == 0) { 
    stopMotors(); 
    return; 
  }

  if (distance > FOLLOW_DISTANCE_CM + FOLLOW_DEAD_ZONE_CM) {
    setMotors(AUTONOMOUS_SPEED, AUTONOMOUS_SPEED);
  } 
  else if (distance < FOLLOW_DISTANCE_CM - FOLLOW_DEAD_ZONE_CM) {
    setMotors(-AUTONOMOUS_SPEED, -AUTONOMOUS_SPEED);
  } 
  else {
    stopMotors();
  }
}

void runEdgeDetect() {
  bool leftOnSurface = (digitalRead(IR_LEFT_PIN) == ON_THE_SURFACE);
  bool rightOnSurface = (digitalRead(IR_RIGHT_PIN) == ON_THE_SURFACE);
  
  if (!leftOnSurface || !rightOnSurface) {
    beepHorn();
    stopMotors(); delay(200);
    setMotors(-AUTONOMOUS_SPEED, -AUTONOMOUS_SPEED); delay(500);
    setMotors(AUTONOMOUS_SPEED, -AUTONOMOUS_SPEED); delay(400);
  } else {
    setMotors(AUTONOMOUS_SPEED, AUTONOMOUS_SPEED);
  }
}

//======================================================================
// --- AUTO PILOT WITH 90 DEGREE TURNS ---
//======================================================================

void runAutoPilot() {
  long distance = getSonarDistance();
  unsigned long stateTime = millis() - autoStateStartTime;
  
  switch (autoState) {
    
    // STATE 1: MOVING FORWARD
    case AUTO_FORWARD:
      setMotors(AUTONOMOUS_SPEED, AUTONOMOUS_SPEED);
      
      // Check for obstacle
      if (distance > 0 && distance < OBSTACLE_DISTANCE_CM) {
        Serial.println("========================================");
        Serial.println("⚠️  OBSTACLE DETECTED at " + String(distance) + "cm");
        Serial.println("========================================");
        beepHorn();
        autoState = AUTO_REVERSING;
        autoStateStartTime = millis();
        turnDirection = random(2); // 0 = left, 1 = right
        Serial.println("Starting avoidance - Direction: " + String(turnDirection == 0 ? "LEFT" : "RIGHT"));
      }
      break;
    
    // STATE 2: REVERSING
    case AUTO_REVERSING:
      setMotors(-AUTONOMOUS_SPEED, -AUTONOMOUS_SPEED);
      
      if (stateTime < 100) {
        // Do nothing - just reversing
      } else if (stateTime % 200 == 0) {
        Serial.println("↩️  Reversing... " + String(600 - stateTime) + "ms remaining");
      }
      
      // Reverse for 600ms
      if (stateTime >= 600) {
        autoState = AUTO_TURNING;
        autoStateStartTime = millis();
        Serial.println("========================================");
        Serial.println("🔄 STARTING 90° " + String(turnDirection == 0 ? "LEFT" : "RIGHT") + " TURN");
        Serial.println("   Duration: " + String(TURN_90_DURATION) + "ms");
        Serial.println("========================================");
      }
      break;
    
    // STATE 3: TURNING 90 DEGREES
    case AUTO_TURNING:
      // PIVOT TURN - One wheel forward, one backward (maximum turning speed)
      if (turnDirection == 0) {
        // LEFT TURN - Left wheel backward, right wheel forward
        setMotors(-AUTONOMOUS_SPEED, AUTONOMOUS_SPEED);
      } else {
        // RIGHT TURN - Left wheel forward, right wheel backward
        setMotors(AUTONOMOUS_SPEED, -AUTONOMOUS_SPEED);
      }
      
      // Turn for calibrated duration (300ms default for 90°)
      if (stateTime >= TURN_90_DURATION) {
        stopMotors();
        delay(100); // Brief stop to stabilize
        autoState = AUTO_CHECKING;
        autoStateStartTime = millis();
        Serial.println("🔄 Turn complete! Checking path...");
      }
      break;
    
    // STATE 4: CHECKING NEW PATH
    case AUTO_CHECKING:
      stopMotors();
      
      // Wait a moment for sonar to stabilize
      if (stateTime < 200) {
        return;
      }
      
      // Check the new path
      long newDistance = getSonarDistance();
      
      Serial.println("========================================");
      Serial.println("👀 CHECKING PATH");
      Serial.println("   Distance: " + String(newDistance) + "cm");
      Serial.println("   Required: >" + String(OBSTACLE_DISTANCE_CM) + "cm");
      
      if (newDistance > OBSTACLE_DISTANCE_CM || newDistance == 0) {
        // Path is clear!
        Serial.println("   Result: ✅ CLEAR - Resuming forward");
        Serial.println("========================================");
        autoState = AUTO_FORWARD;
        autoStateStartTime = millis();
      } 
      else if (stateTime >= 300) {
        // Still blocked, turn another 90 degrees in same direction
        Serial.println("   Result: ❌ BLOCKED - Turning another 90°");
        Serial.println("========================================");
        autoState = AUTO_TURNING;
        autoStateStartTime = millis();
      }
      break;
  }
}
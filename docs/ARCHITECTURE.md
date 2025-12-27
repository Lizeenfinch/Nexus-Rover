# 🏗️ System Architecture - Nexus Rover

## Overview

Nexus Rover employs a **dual-core control architecture** that separates real-time sensor processing from network communication, ensuring deterministic response times and robust safety systems.

---

## 🔧 Dual-Core Control Architecture

### Core 1: Arduino Uno (5V Logic)
**Role**: Real-time sensor processing & motor control

#### Responsibilities
- ⚡ **Real-time Control**: Deterministic response (<1ms sensor loop)
- 🎯 **Motor Management**: PWM speed control and direction logic
- 📡 **Sensor Processing**: Ultrasonic, IR, and voltage monitoring
- 🔊 **Audio System**: Buzzer control and musical horn generation
- 🛡️ **Safety Logic**: Emergency stop and timeout handling
- 📊 **Telemetry**: Sensor data packaging for ESP8266

#### Technical Specifications
- **Microcontroller**: ATmega328P
- **Operating Voltage**: 5V
- **Clock Speed**: 16 MHz
- **Digital I/O**: 14 pins (6 PWM)
- **Analog Inputs**: 6 channels (10-bit ADC)
- **Flash Memory**: 32 KB
- **SRAM**: 2 KB
- **EEPROM**: 1 KB

#### Pin Allocation
```
PWM Outputs (Motor Control):
- Pin 6  (ENB) - Right motor speed
- Pin 11 (ENA) - Left motor speed

Digital Outputs (Motor Direction):
- Pin 7  (IN4) - Right motor direction B
- Pin 8  (IN3) - Right motor direction A
- Pin 9  (IN2) - Left motor direction B
- Pin 10 (IN1) - Left motor direction A

Digital Outputs (Actuators):
- Pin 12 - Buzzer

Digital I/O (Sensors):
- Pin 4 - Ultrasonic TRIG
- Pin 5 - Ultrasonic ECHO

Digital I/O (Communication):
- Pin 2 - SoftwareSerial RX (from ESP)
- Pin 3 - SoftwareSerial TX (to ESP)

Analog Inputs (Sensors):
- A0 - Voltage sensor
- A1 - IR sensor left
- A2 - IR sensor right
```

---

### Core 2: ESP8266 NodeMCU (3.3V Logic)
**Role**: Wi-Fi connectivity, web server, and user interface

#### Responsibilities
- 📡 **WiFi Management**: Network connection and reconnection
- 🌐 **Web Server**: AsyncWebServer on port 80
- 🔌 **WebSocket**: Real-time bidirectional communication
- 🎨 **UI Hosting**: Serves responsive HTML/CSS/JavaScript
- 📨 **Command Relay**: Forwards web commands to Arduino
- 📊 **Telemetry Display**: Receives and broadcasts sensor data

#### Technical Specifications
- **Microcontroller**: ESP-12E (Espressif ESP8266)
- **Operating Voltage**: 3.3V (5V tolerant via onboard regulator)
- **Clock Speed**: 80/160 MHz (selectable)
- **WiFi**: 802.11 b/g/n (2.4 GHz only)
- **Flash Memory**: 4 MB
- **SRAM**: 160 KB
- **GPIO Pins**: 11 usable

#### Pin Allocation
```
Communication (to Arduino):
- D5 (GPIO14) - SoftwareSerial TX (to Arduino RX/Pin 2)
- D6 (GPIO12) - SoftwareSerial RX (from Arduino TX/Pin 3)

Power:
- VIN - 5V input from L298N or Arduino
- GND - Common ground with Arduino
```

---

## ⚡ Power Distribution System

### Cascaded Power Architecture

```
┌─────────────────────────────────────────────────┐
│         3x 18650 Li-ion Cells (12.3V)           │
│              (3.7V × 3 in series)               │
└────────────────┬────────────────────────────────┘
                 │
                 ├─── Power Switch (SPST)
                 │
                 v
        ┌────────────────┐
        │   L298N Driver │
        │   (12V Input)  │
        └────────┬───────┘
                 │
        ┌────────┴────────┬──────────────┐
        │                 │              │
        v                 v              v
  [Motors 12V]    [5V Regulator]   [GND Common]
                        │
                        v
                ┌───────┴───────┐
                │               │
                v               v
         [Arduino 5V]    [ESP8266 VIN]
                │               │
                v               v
           [Sensors]      [3.3V Internal]
```

### Power Distribution Details

#### **High Current Path** (12V)
- **Source**: 3× 18650 cells in series (11.1-12.6V)
- **Capacity**: Typically 2200-3000 mAh
- **Load**: DC motors via L298N (up to 2A per motor)
- **Protection**: Power switch, optional fuse (5A)

#### **Logic Power Path** (5V)
- **Regulation**: L298N onboard 5V regulator (or external buck converter)
- **Current**: ~500 mA available
- **Loads**:
  - Arduino Uno: ~50 mA
  - ESP8266: ~170 mA (WiFi active)
  - Sensors: ~25 mA total
  - Buzzer: ~30 mA (when active)

#### **ESP8266 Internal** (3.3V)
- **Regulation**: Onboard AMS1117-3.3 regulator
- **Input**: 5V from Arduino/L298N
- **Current**: Up to 800 mA capability

---

## 🔄 Communication Protocol

### Inter-Microcontroller Communication

#### **Medium**: SoftwareSerial (115200 baud)
- **Why SoftwareSerial?** Hardware serial (pins 0,1) interferes with USB programming
- **Baud Rate**: 115200 bps (fast, reliable at short distances)
- **Distance**: <30cm between boards (stable)

#### **Safety Considerations**
- **Voltage Levels**: 
  - Arduino TX (5V) → ESP RX (3.3V tolerant on most pins)
  - ESP TX (3.3V) → Arduino RX (recognized as HIGH)
  - *Note: For production, use logic level converter*

### Command Format

#### **Web → ESP → Arduino** (Commands)

```
Format: Single character or M# for mode
Examples:
  'F'  - Forward
  'B'  - Reverse
  'L'  - Turn left 90°
  'R'  - Turn right 90°
  'S'  - Emergency stop
  'H'  - Horn start
  'h'  - Horn stop
  'M0' - Mode: PARK
  'M1' - Mode: LINE FOLLOW
  'M2' - Mode: FOLLOW ME
  'M3' - Mode: EDGE DETECT
  'M4' - Mode: MANUAL
  'M5' - Mode: AUTO PILOT
```

#### **Arduino → ESP → Web** (Telemetry)

```
Format: Prefix:Value
Examples:
  'V:8.2'  - Voltage: 8.2V
  'D:35'   - Distance: 35cm (future)
  'S:150'  - Speed: 150 (future)
```

---

## 🌐 Web Interface Architecture

### Technology Stack

#### **Backend**
- **Framework**: ESPAsyncWebServer
- **Protocol**: HTTP/1.1 + WebSocket
- **Port**: 80 (HTTP)
- **WebSocket Endpoint**: `/ws`

#### **Frontend**
- **HTML5**: Semantic structure
- **CSS3**: Gradients, animations, responsive design
- **JavaScript**: Vanilla JS (no frameworks)
- **Real-time**: WebSocket API

### User Interface Features

#### **Visual Design**
- 🌌 **Starfield Animation**: Dynamic background with shooting stars
- 🎨 **Gradient Theme**: Dark theme with cyan/blue accents
- 📱 **Responsive Layout**: Mobile-first design
- ✨ **Smooth Transitions**: CSS3 animations

#### **Control Elements**
- **Mode Selector**: 6 buttons (grid layout)
- **Steering Section**: Visual steering wheel + turn buttons
- **Pedals Section**: Forward/reverse toggle buttons
- **Utility Bar**: E-STOP and horn buttons
- **Telemetry**: Real-time battery voltage display

#### **Touch Optimization**
- Large touch targets (minimum 44×44px)
- Touch event handling (touchstart/touchend)
- Visual feedback on press
- No accidental clicks (user-select: none)

---

## 🛡️ Safety Systems

### Multi-Layer Safety Architecture

#### **Layer 1: Hardware Emergency Stop**
- Physical E-STOP button in web interface
- Sends 'S' command immediately
- All movement ceases within 50ms
- Cannot be overridden by software

#### **Layer 2: Communication Timeout**
- Monitors last command time
- Auto-stop if no command for 200ms (manual mode)
- Prevents runaway if WiFi drops
- Automatically resumes when reconnected

#### **Layer 3: Turn Auto-Stop**
- 90° turns have fixed duration (300ms default)
- Automatically stops after turn completes
- Prevents continuous spinning
- Returns to previous movement state or stops

#### **Layer 4: Mode Isolation**
- Mode changes trigger immediate stop
- All movement flags reset
- Clears toggle states
- Ensures clean state transitions

#### **Layer 5: Power Monitoring**
- Continuous battery voltage monitoring
- Low voltage warning (future: auto-shutdown)
- Prevents over-discharge damage

### Safety Features in Code

```cpp
// Command timeout check
if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
  stopMotors();
}

// Turn auto-complete
if (isTurning && (millis() - turnStartTime >= MANUAL_TURN_90_DURATION)) {
  isTurning = false;
  stopMotors();
}

// Emergency stop
if (cmd == 'S') {
  stopMotors();
  isMovingForward = false;
  isMovingBackward = false;
  forwardToggle = false;
  reverseToggle = false;
  isTurning = false;
}
```

---

## 🔍 Sensor Integration

### Ultrasonic Distance (HC-SR04)

**Principle**: Time-of-flight measurement

```cpp
// Trigger pulse (10µs)
digitalWrite(SONAR_TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(SONAR_TRIG_PIN, LOW);

// Measure echo pulse width
long duration = pulseIn(SONAR_ECHO_PIN, HIGH, 25000);

// Calculate distance
// Speed of sound: 343 m/s = 0.034 cm/µs
// Divide by 2 for round trip
long distance = duration * 0.034 / 2;
```

**Range**: 2-400 cm  
**Accuracy**: ±3mm  
**Update Rate**: ~20 Hz  
**Use Cases**: Obstacle detection, follow-me mode

---

### IR Obstacle Sensors (Dual)

**Principle**: Infrared reflection detection

**Configuration**:
- **Digital Output**: HIGH when obstacle/line detected
- **Sensitivity**: Adjustable via potentiometer
- **Range**: 2-30 cm

**Applications**:
- **Line Following**: Detects black line vs white surface
- **Edge Detection**: Detects surface vs void

```cpp
// Read both sensors
bool leftOnLine = (digitalRead(IR_LEFT_PIN) == ON_THE_LINE);
bool rightOnLine = (digitalRead(IR_RIGHT_PIN) == ON_THE_LINE);

// Line following logic
if (leftOnLine && !rightOnLine) {
  // Turn right (line under left sensor only)
  setMotors(SPEED, SPEED/2);
} else if (!leftOnLine && rightOnLine) {
  // Turn left (line under right sensor only)
  setMotors(SPEED/2, SPEED);
}
```

---

### Voltage Monitoring

**Principle**: Resistive voltage divider

```cpp
// ADC reading (0-1023 for 0-5V)
int reading = analogRead(VOLTAGE_SENSE_PIN);

// Convert to voltage
// 5.0V reference / 1023 steps = ADC voltage
// Multiply by divider ratio (typically 5:1)
float voltage = reading * (5.0 / 1023.0) * VOLTAGE_SENSOR_MULTIPLIER;
```

**Purpose**:
- Battery health monitoring
- Low voltage warnings
- Runtime estimation

---

## 🎮 Control Modes Implementation

### State Machine Architecture

```cpp
enum DriveMode {
  PARK,         // 0 - Stationary
  LINE_FOLLOW,  // 1 - IR line tracking
  FOLLOW_ME,    // 2 - Distance maintaining
  EDGE_DETECT,  // 3 - Edge avoidance
  MANUAL,       // 4 - WiFi control
  AUTO_PILOT    // 5 - Autonomous navigation
};

DriveMode currentMode = PARK;

void loop() {
  // Process commands from ESP8266
  if (esp8266.available()) parseCommand();
  
  // Run current mode logic
  runCurrentMode();
  
  // Send telemetry back to ESP
  sendTelemetry();
}
```

Each mode has dedicated logic in `runCurrentMode()` function.

---

## 📊 Performance Characteristics

### Response Times

| Event | Latency | Notes |
|-------|---------|-------|
| Sensor Reading | <1 ms | Arduino loop |
| Motor Response | <10 ms | PWM update |
| Command Processing | <50 ms | Web → Motors |
| WebSocket Update | <100 ms | Round trip |
| Emergency Stop | <50 ms | Highest priority |

### Bandwidth

| Data Flow | Rate | Bandwidth |
|-----------|------|-----------|
| Commands (Web → Arduino) | ~10 Hz | <100 bytes/s |
| Telemetry (Arduino → Web) | 1 Hz | ~50 bytes/s |
| Total Network | - | <1 kbps |

**Conclusion**: Very low bandwidth requirements allow stable operation even with weak WiFi signal.

---

## 🔄 System Workflow

### Startup Sequence

1. **Power On**
   - Battery → L298N → 5V regulators
   - Arduino boots (~2s)
   - ESP8266 boots (~3s)

2. **Initialization**
   - Arduino: Configure pins, initialize serial
   - ESP8266: Connect to WiFi (5-10s)

3. **Ready State**
   - Web server active
   - Arduino in PARK mode
   - Waiting for commands

### Operational Loop

```
┌─────────────────────────────────────────┐
│         User Opens Web Interface        │
└───────────────┬─────────────────────────┘
                │
                v
┌─────────────────────────────────────────┐
│    User Selects Mode / Sends Command    │
└───────────────┬─────────────────────────┘
                │
                v
        ┌───────────────┐
        │  WebSocket TX  │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  ESP8266 RX   │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ SoftwareSerial│
        │      TX       │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  Arduino RX   │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ Parse Command │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ Execute Mode  │
        │    Logic      │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ Control Motors│
        │ Read Sensors  │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │   Telemetry   │
        │   (Voltage)   │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ Arduino TX    │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  ESP8266 RX   │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │  WebSocket TX │
        └───────┬───────┘
                │
                v
        ┌───────────────┐
        │ Update Web UI │
        └───────────────┘
```

---

## 🚀 Future Architecture Evolution

### Phase 2: Industrial Prototype

**Planned Enhancements**:

#### **Processing Power**
- Upgrade to **Raspberry Pi 4** or **Jetson Nano**
- Keep Arduino for motor control (proven reliability)
- Pi handles AI, vision, mapping

#### **Connectivity**
- 5G/LTE modem for unlimited range
- LoRaWAN for low-power long-range
- Mesh networking for multi-robot coordination

#### **Perception**
- 360° LiDAR (SLAM mapping)
- Stereo camera (depth perception)
- IMU/Gyroscope (stability)
- GPS (outdoor navigation)

#### **Actuation**
- Upgrade to 43A motor drivers
- Ackermann steering with servo
- Increased payload capacity (35kg)

---

## 📖 Architecture Documentation

**Design Principles**:
1. ✅ **Separation of Concerns**: Real-time vs networking
2. ✅ **Fail-Safe Design**: Multiple safety layers
3. ✅ **Modularity**: Easy to upgrade components
4. ✅ **Scalability**: Clear path to industrial version
5. ✅ **Accessibility**: Standard components, open source

**Key Decisions**:
- **Why dual-core?** Separates critical timing from networking
- **Why SoftwareSerial?** Preserves USB programming capability
- **Why WebSocket?** Real-time with low overhead
- **Why Arduino?** Proven reliability for real-time control

---

**This architecture has been validated in the $50 proof-of-concept and forms the foundation for the $800 industrial prototype.**

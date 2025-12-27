# 🔧 Hardware Documentation

## Table of Contents
- [Bill of Materials](#bill-of-materials)
- [Detailed Component Specifications](#detailed-component-specifications)
- [Wiring Schematic](#wiring-schematic)
- [Assembly Instructions](#assembly-instructions)
- [Power Requirements](#power-requirements)

## Bill of Materials

### Core Components

| Quantity | Component | Specification | Estimated Cost |
|----------|-----------|---------------|----------------|
| 1 | Arduino Uno/Nano | ATmega328P, 5V, 16MHz | $5-25 |
| 1 | ESP8266 NodeMCU | ESP-12E, WiFi 802.11 b/g/n | $3-8 |
| 1 | L298N Motor Driver | Dual H-Bridge, 5-35V, 2A per channel | $2-5 |
| 2 | DC Geared Motors | 6-12V, 200-300 RPM | $3-8 |
| 1 | HC-SR04 Ultrasonic Sensor | Range: 2-400cm | $1-3 |
| 2 | IR Obstacle Sensor | Digital output, adjustable distance | $1-2 each |
| 1 | Voltage Sensor Module | 0-25V measurement range | $1-2 |
| 1 | Buzzer | 5V Active or Passive | $0.50-1 |
| 1 | Robot Car Chassis | 2WD or 4WD with wheels | $8-15 |
| 1 | Battery Holder | For 2x 18650 or 6x AA | $2-5 |
| 2 | 18650 Batteries | 3.7V, 2200mAh+ (or 6x AA) | $5-10 |
| - | Jumper Wires | Male-Male, Male-Female, Female-Female | $3-5 |
| 1 | Power Switch | SPST, 2A+ | $1-2 |
| - | Mounting Hardware | Screws, standoffs, zip ties | $2-5 |

**Total Estimated Cost: $40-100** (depending on quality and source)

---

## Detailed Component Specifications

### 1. Arduino Uno/Nano
**Purpose:** Main controller for sensor processing and motor control

**Specifications:**
- Microcontroller: ATmega328P
- Operating Voltage: 5V
- Digital I/O Pins: 14 (6 PWM)
- Analog Input Pins: 6
- Flash Memory: 32 KB
- SRAM: 2 KB
- Clock Speed: 16 MHz

**Alternatives:** Arduino Pro Mini, Arduino Mega (if expanding)

---

### 2. ESP8266 NodeMCU
**Purpose:** WiFi connectivity and web server

**Specifications:**
- Microcontroller: ESP-12E
- WiFi: 802.11 b/g/n (2.4GHz only)
- Operating Voltage: 3.3V (onboard regulator accepts 5V via USB)
- GPIO Pins: 11 usable
- Flash Memory: 4 MB
- Clock Speed: 80/160 MHz

**Important Notes:**
- ESP8266 GPIO pins are 3.3V tolerant only
- Use voltage divider or level shifter for 5V Arduino communication (though direct connection works in practice with SoftwareSerial)

---

### 3. L298N Motor Driver
**Purpose:** Control two DC motors with speed and direction

**Specifications:**
- Supply Voltage: 5-35V
- Logic Voltage: 5V
- Max Current: 2A per channel (4A total)
- PWM Control: Supported for speed control
- Built-in 5V Regulator: Can power Arduino (if Vin > 12V, remove jumper)

**Connections:**
- ENA, ENB: PWM speed control
- IN1, IN2: Motor A direction
- IN3, IN4: Motor B direction
- OUT1, OUT2: Motor A terminals
- OUT3, OUT4: Motor B terminals

---

### 4. DC Geared Motors
**Purpose:** Rover movement

**Recommended Specifications:**
- Voltage: 6-12V
- Speed: 200-300 RPM (at 12V)
- Torque: 2-3 kg-cm
- Gear Ratio: 1:48 or 1:90
- Shaft Diameter: 3mm or 5mm

**Note:** Higher torque is better for obstacle climbing

---

### 5. HC-SR04 Ultrasonic Sensor
**Purpose:** Obstacle detection and distance measurement

**Specifications:**
- Operating Voltage: 5V
- Sensing Range: 2cm - 400cm
- Accuracy: ±3mm
- Measuring Angle: 15°
- Trigger Signal: 10µs TTL pulse
- Echo Signal: Proportional to distance

**Pins:**
- VCC: 5V
- GND: Ground
- TRIG: Trigger input
- ECHO: Echo output

---

### 6. IR Obstacle Sensors
**Purpose:** Line following and edge detection

**Specifications:**
- Operating Voltage: 3.3-5V
- Detection Distance: 2-30cm (adjustable via potentiometer)
- Output: Digital (HIGH when obstacle detected)
- Detection Angle: ~35°

**Configuration:**
- Line Following: Adjust to detect black/white contrast
- Edge Detection: Adjust to detect floor vs. void

---

### 7. Voltage Sensor Module
**Purpose:** Battery level monitoring

**Specifications:**
- Input Voltage Range: 0-25V DC
- Output Voltage: 0-5V (Arduino-safe)
- Voltage Divider Ratio: 5:1 (typically)
- Resolution: ~0.02V

**Formula:**
```
Actual Voltage = (Analog Reading / 1023) * 5.0 * Multiplier
```

---

### 8. Power System

**Battery Options:**

**Option 1: 2x 18650 Lithium-Ion (Recommended)**
- Voltage: 7.4V nominal (8.4V full, 6V empty)
- Capacity: 2200-3000mAh
- Weight: ~90g
- Runtime: 1-2 hours

**Option 2: 6x AA Batteries**
- Voltage: 9V nominal (1.5V × 6)
- Capacity: 2000-3000mAh (alkaline)
- Weight: ~140g
- Runtime: 1-2 hours

**Option 3: 3S LiPo Battery**
- Voltage: 11.1V nominal
- Capacity: 1000-2200mAh
- Weight: 80-150g
- Runtime: 1-2 hours
- **Requires LiPo charger and protection!**

---

## Wiring Schematic

### Power Distribution

```
Battery (+) ─┬─→ Switch ─→ L298N (12V)
             │
             ├─→ L298N 5V Out ─→ Arduino VIN
             │
             └─→ Arduino VIN ─→ ESP8266 VIN (5V)

Battery (-) ─→ Common Ground (GND)
```

### Arduino Pin Assignments

```
DIGITAL PINS:
Pin 2  → ESP8266 TX (Software Serial RX)
Pin 3  → ESP8266 RX (Software Serial TX)
Pin 4  → HC-SR04 TRIG
Pin 5  → HC-SR04 ECHO
Pin 6  → L298N ENB (PWM - Right Motor Speed)
Pin 7  → L298N IN4 (Right Motor Direction)
Pin 8  → L298N IN3 (Right Motor Direction)
Pin 9  → L298N IN2 (Left Motor Direction)
Pin 10 → L298N IN1 (Left Motor Direction)
Pin 11 → L298N ENA (PWM - Left Motor Speed)
Pin 12 → Buzzer (+)

ANALOG PINS:
A0 → Voltage Sensor (Signal)
A1 → IR Sensor Left (OUT)
A2 → IR Sensor Right (OUT)
```

### ESP8266 Pin Assignments

```
D5 (GPIO14) → Arduino Pin 3 (TX)
D6 (GPIO12) → Arduino Pin 2 (RX)
VIN         → 5V from Arduino/L298N
GND         → Common Ground
```

### L298N Connections

```
POWER:
12V  → Battery (+) via switch
GND  → Battery (-)
5V   → Arduino VIN (if using onboard regulator)

LOGIC (from Arduino):
ENA  → Pin 11 (PWM)
IN1  → Pin 10
IN2  → Pin 9
ENB  → Pin 6 (PWM)
IN3  → Pin 8
IN4  → Pin 7

MOTOR OUTPUTS:
OUT1 → Left Motor (+)
OUT2 → Left Motor (-)
OUT3 → Right Motor (+)
OUT4 → Right Motor (-)
```

---

## Assembly Instructions

### Step 1: Chassis Preparation
1. Assemble the robot chassis according to manufacturer instructions
2. Mount motors to chassis with screws
3. Attach wheels to motor shafts
4. Install caster wheel or ball bearing (for 2WD chassis)

### Step 2: Component Mounting
1. Mount L298N motor driver on top deck using standoffs or zip ties
2. Mount Arduino on chassis using standoffs or double-sided tape
3. Mount ESP8266 near Arduino (but away from motors to reduce interference)
4. Mount HC-SR04 sensor at front center, facing forward
5. Mount IR sensors at front bottom, facing downward (45° angle recommended)
6. Mount buzzer in accessible location
7. Mount battery holder on bottom deck

### Step 3: Wiring - Power First
1. **POWER OFF** - Ensure switch is OFF and batteries removed
2. Connect battery holder (+) to switch input
3. Connect switch output to L298N 12V input
4. Connect battery holder (-) to L298N GND
5. **Establish common ground** - Connect all GND pins together

### Step 4: Wiring - Motor Driver
1. Connect L298N control pins to Arduino (see pin assignments above)
2. Connect motor wires to L298N outputs (OUT1-OUT4)
3. If motors run backward, swap the two wires for that motor

### Step 5: Wiring - Sensors
1. Connect HC-SR04:
   - VCC → Arduino 5V
   - GND → Common GND
   - TRIG → Arduino Pin 4
   - ECHO → Arduino Pin 5

2. Connect IR Sensors:
   - VCC → Arduino 5V
   - GND → Common GND
   - OUT (Left) → Arduino A1
   - OUT (Right) → Arduino A2

3. Connect Voltage Sensor:
   - (+) → Battery (+) after switch
   - (-) → Common GND
   - S → Arduino A0

### Step 6: Wiring - Communication
1. Arduino Pin 2 → ESP8266 D6 (GPIO12)
2. Arduino Pin 3 → ESP8266 D5 (GPIO14)
3. ESP8266 GND → Common GND
4. ESP8266 VIN → 5V from L298N or Arduino

### Step 7: Wiring - Buzzer
1. Buzzer (+) → Arduino Pin 12
2. Buzzer (-) → Common GND

### Step 8: Cable Management
1. Use zip ties to secure wires along chassis
2. Keep motor wires away from signal wires
3. Ensure no wires touch moving parts (wheels, gears)
4. Label wires for future troubleshooting

### Step 9: Testing
1. **Visual inspection** - Check all connections
2. **Continuity test** - Verify no shorts with multimeter
3. **Power test** - Insert batteries and turn on
   - Arduino LED should light up
   - ESP8266 LED should blink during boot
4. **Upload code** and test each function individually

---

## Power Requirements

### Current Draw Estimates

| Component | Idle Current | Active Current |
|-----------|--------------|----------------|
| Arduino Uno | 45 mA | 50 mA |
| ESP8266 | 80 mA | 170 mA (WiFi active) |
| Motors (each) | 0 mA | 200-500 mA |
| HC-SR04 | 2 mA | 15 mA |
| IR Sensors (2x) | 3 mA each | 3 mA each |
| Buzzer | 0 mA | 30 mA |

**Total Current:**
- Idle (no motors): ~140 mA
- Light use (slow speed): ~500 mA
- Full throttle: ~1.5 A

**Battery Life Calculation:**
```
Runtime = Battery Capacity (mAh) / Average Current (mA)
Example: 2500 mAh / 800 mA average = ~3 hours
```

---

## Safety Considerations

⚠️ **Important Safety Notes:**

1. **Battery Safety**
   - Never short circuit batteries
   - Use protection circuits for LiPo batteries
   - Don't over-discharge batteries (stop at 6V for 2S, 9V for 3S)
   - Charge batteries safely using appropriate charger

2. **Wiring Safety**
   - Double-check polarity before powering on
   - Use appropriate wire gauge (22-24 AWG for signals, 18-20 AWG for power)
   - Insulate all exposed connections
   - Add a fuse (2A) in the power line for extra protection

3. **Motor Driver Safety**
   - Ensure heatsink on L298N is not blocked
   - Don't exceed 2A per channel
   - Module may get warm during extended use (this is normal)

4. **Operation Safety**
   - Test in open area first
   - Keep away from stairs/ledges until edge detection is verified
   - Have emergency stop easily accessible
   - Supervise autonomous modes

---

## Troubleshooting Hardware

### Motors Don't Move
- ✓ Check battery voltage (should be > 7V)
- ✓ Check L298N power LED
- ✓ Verify motor connections to L298N
- ✓ Test motors directly with battery
- ✓ Check Arduino pin connections to L298N

### ESP8266 Won't Boot
- ✓ Check power supply (needs stable 5V)
- ✓ Verify USB cable supports data (some are charge-only)
- ✓ Press RESET button
- ✓ Check for short circuits

### Sensors Not Responding
- ✓ Verify 5V power to sensors
- ✓ Check sensor orientation
- ✓ Test sensor individually in Serial Monitor
- ✓ Adjust IR sensor sensitivity (onboard potentiometer)

### Rover Turns Incorrectly
- ✓ Check motor direction (swap wires if backward)
- ✓ Verify left/right motor assignments match code
- ✓ Check battery level (low voltage = slower turns)
- ✓ Adjust turn duration in code

---

## Upgrades & Modifications

### Possible Enhancements
- Add servo for scanning sonar
- Install LED headlights/taillights
- Add MPU6050 for gyroscope stabilization
- Install camera module (ESP32-CAM)
- Add encoders for precise distance tracking
- Implement Bluetooth control (HC-05 module)
- Add LCD display for status
- Install OLED screen on rover

---

**Need help? Open an issue on GitHub!**

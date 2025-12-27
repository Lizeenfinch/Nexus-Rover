# 🚀 Quick Start Guide

Get your Nexus Rover up and running in 30 minutes!

## What You'll Need

### Hardware
- ✅ Arduino Uno or Nano
- ✅ ESP8266 NodeMCU
- ✅ L298N Motor Driver
- ✅ 2x DC Motors with wheels
- ✅ HC-SR04 Ultrasonic sensor
- ✅ 2x IR sensors
- ✅ Buzzer
- ✅ Battery (7.4V-12V)
- ✅ Jumper wires

### Software
- ✅ Arduino IDE ([Download Here](https://www.arduino.cc/en/software))
- ✅ USB Cable (for programming)
- ✅ WiFi Network (2.4GHz)

---

## Step 1: Install Arduino IDE (5 min)

1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install and open Arduino IDE
3. Add ESP8266 board support:
   - Go to `File` → `Preferences`
   - In "Additional Board Manager URLs" add:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Click OK
   - Go to `Tools` → `Board` → `Boards Manager`
   - Search "ESP8266" and click Install

---

## Step 2: Install Libraries for ESP8266 (3 min)

Download and install these libraries:

1. **ESPAsyncTCP**
   - Download: https://github.com/me-no-dev/ESPAsyncTCP
   - `Sketch` → `Include Library` → `Add .ZIP Library`

2. **ESPAsyncWebServer**
   - Download: https://github.com/me-no-dev/ESPAsyncWebServer
   - `Sketch` → `Include Library` → `Add .ZIP Library`

---

## Step 3: Basic Wiring (15 min)

### Quick Connection Guide

**Power First:**
```
Battery (+) → Switch → L298N (12V)
Battery (-) → L298N (GND)
L298N (5V) → Arduino (VIN)
```

**Motors:**
```
Left Motor  → L298N OUT1, OUT2
Right Motor → L298N OUT3, OUT4
```

**Arduino to L298N:**
```
Pin 11 → ENA    Pin 6  → ENB
Pin 10 → IN1    Pin 8  → IN3
Pin 9  → IN2    Pin 7  → IN4
```

**Sensors to Arduino:**
```
Ultrasonic:  TRIG→Pin4, ECHO→Pin5
IR Left:     OUT→A1
IR Right:    OUT→A2
Buzzer:      (+)→Pin12
```

**Arduino to ESP8266:**
```
Arduino Pin 2 → ESP D6 (GPIO12)
Arduino Pin 3 → ESP D5 (GPIO14)
GND → GND (common ground!)
```

> 💡 **Tip:** Use different wire colors - Red for power, Black for ground, other colors for signals

---

## Step 4: Upload Arduino Code (3 min)

1. Open `project_cararduino/project_cararduino.ino`
2. Select board: `Tools` → `Board` → `Arduino Uno` (or Nano)
3. Select port: `Tools` → `Port` → `COM#` 
4. Click **Upload** button (→)
5. Wait for "Done uploading"

**Troubleshooting:**
- If error occurs, disconnect ESP8266 from pins 2 & 3 during upload
- For Nano: Select `Tools` → `Processor` → `ATmega328P (Old Bootloader)`

---

## Step 5: Configure WiFi & Upload ESP Code (4 min)

1. Open `project_caresp/project_caresp.ino`

2. **IMPORTANT:** Update WiFi credentials (lines 6-7):
   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";      // Change this!
   const char* password = "YOUR_PASSWORD";    // Change this!
   ```

3. Select board: `Tools` → `Board` → `NodeMCU 1.0 (ESP-12E Module)`

4. Select port: `Tools` → `Port` → `COM#` (different from Arduino)

5. Click **Upload**

6. Open Serial Monitor: `Tools` → `Serial Monitor`
   - Set baud rate to `115200`
   - Wait for WiFi connection message
   - **Note the IP address shown** (e.g., 192.168.1.100)

---

## Step 6: Test the Rover! (5 min)

### Power On
1. Turn off switch
2. Insert charged batteries
3. Turn on switch
4. Wait 10 seconds for ESP8266 to connect

### Connect to Web Interface
1. Open web browser on phone/computer
2. Make sure you're on the **same WiFi network**
3. Type the IP address from Step 5 (e.g., `192.168.1.100`)
4. You should see the **Nexus Drive** dashboard!

### First Movement Test
1. Click **MANUAL** mode
2. Click **FORWARD** button → Rover should move forward
3. Click **FORWARD** again → Rover should stop
4. Click **LEFT** → Rover should turn left 90°
5. Click **E-STOP** → Everything stops

### Test Autonomous Modes
1. **LINE FOLLOW** - Place on black line (electrical tape on white surface)
2. **FOLLOW ME** - Place hand 20cm in front
3. **EDGE DETECT** - Use on table (will stop at edge)
4. **AUTO PILOT** - Fully autonomous navigation

---

## Common Quick Start Issues

### ❌ "ESP8266 won't connect to WiFi"
- ✅ Check SSID and password (case-sensitive!)
- ✅ Ensure it's a 2.4GHz network (not 5GHz)
- ✅ Move closer to router

### ❌ "Motors don't move"
- ✅ Check battery voltage (should be >7V)
- ✅ Verify all connections
- ✅ Check power switch is ON

### ❌ "Web page won't load"
- ✅ Confirm you're on same WiFi network
- ✅ Check IP address in Serial Monitor
- ✅ Try `http://` before IP (e.g., `http://192.168.1.100`)

### ❌ "Code won't upload"
- ✅ Select correct board and port
- ✅ Check USB cable (some are charge-only)
- ✅ Disconnect ESP from Arduino pins 0,1 during upload

---

## What's Next?

### Calibration
- **Adjust turn duration** - Edit `TURN_90_DURATION` for accurate 90° turns
- **Tune speed** - Modify `MANUAL_SPEED` and `AUTONOMOUS_SPEED`
- **Set sensors** - Adjust IR sensitivity with potentiometers

### Learning
- Read [HARDWARE.md](docs/HARDWARE.md) for detailed wiring
- Check [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) for solutions
- Explore code comments to understand how it works

### Customization
- Change web interface colors
- Add more sensors
- Create custom autonomous behaviors
- Design 3D printed chassis

---

## Safety Tips

⚠️ **Important:**
- Don't run near stairs until edge detection is tested
- Keep emergency stop button accessible
- Don't leave powered on unattended
- Disconnect battery when working on wiring

---

## Need Help?

- 📖 Read [full documentation](../README.md)
- 🔧 Check [troubleshooting guide](docs/TROUBLESHOOTING.md)
- 💬 Open GitHub issue
- 📺 Search YouTube for "Arduino robot car" tutorials

---

**Congratulations! Your Nexus Rover is ready to explore! 🎉🤖**

---

## Quick Reference Card

```
┌─────────────────────────────────────┐
│        NEXUS ROVER CONTROLS         │
├─────────────────────────────────────┤
│ MODES:                              │
│  0 = PARK       (stopped)           │
│  1 = LINE       (line following)    │
│  2 = FOLLOW     (follow object)     │
│  3 = EDGE       (edge detection)    │
│  4 = MANUAL     (WiFi control)      │
│  5 = AUTO       (autonomous)        │
├─────────────────────────────────────┤
│ MANUAL CONTROLS:                    │
│  FORWARD  - Toggle movement         │
│  REVERSE  - Toggle reverse          │
│  LEFT     - 90° turn left           │
│  RIGHT    - 90° turn right          │
│  E-STOP   - Emergency stop          │
│  HORN     - Musical horn            │
└─────────────────────────────────────┘
```

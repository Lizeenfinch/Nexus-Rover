# 🤖 Nexus Rover

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-00979D?logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![ESP8266](https://img.shields.io/badge/ESP8266-E7352C?logo=espressif&logoColor=white)](https://www.espressif.com/)

### *A Cost-Effective, Wi-Fi Enabled Autonomous Mobile Robot for Industrial & Hazardous Applications*

An advanced autonomous rover with WiFi control, featuring multiple driving modes, obstacle avoidance, line following, and edge detection capabilities.

---

## 🎯 The Challenge & Solution

### The Problem
Industrial Autonomous Mobile Robots (AMRs) typically cost **>$5,000**, creating a significant barrier to entry for small businesses and educational institutions. Traditional robots require heavy infrastructure and suffer from connectivity limitations like Bluetooth's limited range (~10m).

### The Nexus Solution
We developed a **sub-$50 proof-of-concept architecture** that validates industrial logic. By using **Wi-Fi (ESP8266)** instead of Bluetooth, we enable:
- ✅ **Long-range control** (100m+ vs 10m Bluetooth)
- ✅ **Cloud scalability** potential
- ✅ **Web-based interface** accessible from any device
- ✅ **Industrial automation logic** at a fraction of the cost

**This repository demonstrates that professional-grade autonomous robotics can be accessible to everyone.**

## 🌟 Features

### 🎮 **6 Operational Intelligence Modes**

#### **🅿️ PARK Mode**
Safe standby state with all systems ready

#### **🕹️ MANUAL Mode**
Low-latency WebSocket controller with safety auto-stop timeout. Direct control from any web browser with real-time response.

#### **🤖 AUTO PILOT Mode**
Obstacle avoidance using ultrasonic logic (<4m redirect) with intelligent pathfinding and 90° precision turns.

#### **📍 LINE FOLLOW Mode**
Dual IR sensors for precise path tracking in warehouse lanes and guided navigation applications.

#### **🔍 EDGE DETECT Mode**
Downward sensors avoid falls from platforms or stairs - essential for elevated workspace safety.

#### **🚶 FOLLOW ME Mode**
Maintains programmed distance from a moving target up to 4m. Perfect for material transport and human-robot collaboration.

### 🎯 **Control Features**
- **WiFi Web Interface** - Beautiful responsive web dashboard
- **Real-time Telemetry** - Battery voltage monitoring
- **90° Precision Turns** - Calibrated pivot turning for accurate navigation
- **Toggle Controls** - Single-tap forward/reverse with automatic stop
- **Musical Horn** - Multi-tone horn system
- **Emergency Stop** - Instant safety stop button

### 🔧 **Technical Highlights**

#### **Dual-Core Control Architecture**
- **Arduino Uno (5V)**: Real-time sensor processing & motor control with deterministic response
- **ESP8266 (3.3V)**: Handles Wi-Fi, web server, and user interface
- **Safety**: Isolated communication protects sensitive ESP8266 circuitry

#### **Cascaded Power Distribution**
- **Source**: 3x 18650 Li-ion cells (12.3V nominal)
- **High current path**: Direct to motors via L298N
- **Regulated 5V**: Buck converter for logic circuits and sensors

#### **Control Systems**
- WebSocket-based real-time communication (<50ms latency)
- Adaptive speed control with anti-stall logic
- Sonar-based obstacle detection (2-400cm range)
- IR-based line and edge detection with adjustable sensitivity
- PWM motor control with L298N H-bridge driver (2A per channel)

## 📦 Hardware Requirements

### **Microcontrollers**
- Arduino Uno/Nano (Main controller)
- ESP8266 NodeMCU (WiFi module)

### **Motors & Drivers**
- L298N Motor Driver Module
- 2x DC Geared Motors (with wheels)

### **Sensors**
- HC-SR04 Ultrasonic Sensor
- 2x IR Obstacle/Line Sensor Modules
- Voltage Sensor Module (0-25V)

### **Other Components**
- Buzzer (Active/Passive)
- Chassis (2WD/4WD robot car chassis)
- Battery Pack (7.4V-12V recommended)
- Jumper wires
- Power switch

## 🔌 Wiring Diagram

### **Arduino Connections**
```
Motor Driver (L298N):
- ENA  → Pin 11 (PWM)
- IN1  → Pin 10
- IN2  → Pin 9
- ENB  → Pin 6 (PWM)
- IN3  → Pin 8
- IN4  → Pin 7

Sensors:
- Ultrasonic TRIG → Pin 4
- Ultrasonic ECHO → Pin 5
- IR Right        → A2
- IR Left         → A1
- Voltage Sensor  → A0

ESP8266:
- ESP RX → Pin 2
- ESP TX → Pin 3

Buzzer:
- Buzzer → Pin 12
```

### **ESP8266 Connections**
```
Arduino Communication:
- D6 (GPIO12) → Arduino TX (Pin 3)
- D5 (GPIO14) → Arduino RX (Pin 2)
```

## 🚀 Installation & Setup

### **1. Arduino Setup**

1. Install the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install required library:
   - SoftwareSerial (Built-in)

3. Upload the Arduino code:
   - Open `project_cararduino/project_cararduino.ino`
   - Select your Arduino board and port
   - Click Upload

### **2. ESP8266 Setup**

1. Install ESP8266 board support:
   - File → Preferences
   - Add to "Additional Board Manager URLs": 
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Tools → Board → Boards Manager → Search "ESP8266" → Install

2. Install required libraries:
   - **ESP8266WiFi** (Built-in with ESP8266 board)
   - **ESPAsyncTCP** - [Download](https://github.com/me-no-dev/ESPAsyncTCP)
   - **ESPAsyncWebServer** - [Download](https://github.com/me-no-dev/ESPAsyncWebServer)

3. Configure WiFi credentials:
   - Open `project_caresp/project_caresp.ino`
   - Update lines 6-7 with your WiFi credentials:
     ```cpp
     const char* ssid = "Your_WiFi_Name";
     const char* password = "Your_WiFi_Password";
     ```

4. Upload the ESP8266 code:
   - Select Board: "NodeMCU 1.0 (ESP-12E Module)"
   - Select your ESP8266 port
   - Click Upload

### **3. Power Up & Connect**

1. Power the rover with battery
2. Wait for ESP8266 to connect to WiFi (~5-10 seconds)
3. Check Serial Monitor for IP address
4. Open web browser and navigate to the displayed IP address
5. Start controlling your rover! 🎉

## 🎮 How to Use

### **Web Interface**

Once connected, you'll see the **Nexus Drive** dashboard with:

1. **Mode Selection Bar** - Choose between 6 driving modes
2. **Steering Section** - Left/Right 90° turn buttons
3. **Pedals Section** - Forward/Reverse toggle controls
4. **Emergency Stop** - Instant safety stop
5. **Musical Horn** - Press and hold for melody
6. **Battery Monitor** - Real-time voltage display

### **Manual Mode Controls**

- **Forward/Reverse** - Click once to START, click again to STOP (toggle mode)
- **Left/Right** - Single click executes precise 90° turn and auto-stops
- **E-STOP** - Immediately stops all movement
- **Horn** - Press and hold for musical tune

### **Autonomous Modes**

#### **Line Follow Mode**
- Place rover on a black line (on white surface)
- Rover automatically follows the line
- Stops when both sensors detect the line end

#### **Follow Me Mode**
- Rover maintains distance to object in front
- Automatically moves forward/backward to keep ~20cm distance

#### **Edge Detect Mode**
- Safe for table-top operation
- Detects edges and automatically reverses and turns

#### **Auto Pilot Mode**
- Fully autonomous navigation
- Detects obstacles at 35cm
- Executes 90° avoidance maneuvers
- Continues exploring until clear path found

## ⚙️ Configuration & Tuning

Key tunable parameters in `project_cararduino.ino`:

```cpp
// Speed Settings
const int MANUAL_SPEED = 200;          // Manual mode speed (0-255)
const int AUTONOMOUS_SPEED = 130;      // Auto mode speed

// Turn Calibration
const int TURN_90_DURATION = 300;      // Milliseconds for 90° turn (adjust for your rover)

// Obstacle Detection
const int OBSTACLE_DISTANCE_CM = 35;   // Obstacle detection distance
const int FOLLOW_DISTANCE_CM = 20;     // Follow-me target distance

// Voltage Sensor
const float VOLTAGE_SENSOR_MULTIPLIER = 5.0;  // Adjust based on your voltage divider
```

### **Calibrating 90° Turns**

1. Set rover on flat surface
2. Mark starting position and direction
3. Execute a turn in manual mode
4. Measure actual angle
5. Adjust `TURN_90_DURATION`:
   - Too short → Increase value
   - Too long → Decrease value
6. Re-upload and test


## 🧩 Project Structure

```
project_car/
├── README.md                           # This file - Project overview
├── LICENSE                             # MIT License
├── .gitignore                          # Git ignore rules
├── CHANGELOG.md                        # Version history
│
├── docs/
│   ├── QUICK_START.md                  # 30-minute setup guide
│   ├── HARDWARE.md                     # Component specs & wiring
│   ├── ARCHITECTURE.md                 # System design & technical details
│   ├── MARKET_ANALYSIS.md              # Competitive analysis & positioning
│   ├── TROUBLESHOOTING.md              # Common issues & solutions
│   ├── CONTRIBUTING.md                 # Contribution guidelines
│   ├── GITHUB_SETUP.md                 # How to publish repository
│   └── REPOSITORY_STRUCTURE.md         # Documentation overview
│
├── images/
│   └── README.md                       # Media guidelines
│
├── project_cararduino/
│   └── project_cararduino.ino          # Arduino main controller (533 lines)
│
└── project_caresp/
    └── project_caresp.ino              # ESP8266 WiFi module (352 lines)
```

## 🔧 Troubleshooting

### **ESP8266 Won't Connect**
- Check WiFi credentials
- Ensure 2.4GHz WiFi (ESP8266 doesn't support 5GHz)
- Check power supply (ESP8266 needs stable 3.3V)

### **Motors Not Moving**
- Check motor driver connections
- Verify battery voltage (should be 7-12V)
- Test motor driver separately

### **Sensors Not Working**
- Verify sensor power connections
- Check sensor orientation
- Test sensors individually in Serial Monitor

### **Turn Angle Incorrect**
- Adjust `TURN_90_DURATION` parameter
- Check battery voltage (low voltage = slower turns)
- Ensure wheels have good traction

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation

Please read [CONTRIBUTING.md](docs/CONTRIBUTING.md) for details.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## � Scalability & Future Roadmap

### Phase 1: Proof of Concept (Current - $50)
✅ Basic autonomous navigation  
✅ Wi-Fi control and web interface  
✅ Core operational modes validated  
✅ Sub-$50 component cost achieved  

### Phase 2: Industrial Prototype ($800 Target)

#### **Advanced Perception**
- 🎯 360° LiDAR integration for SLAM (Simultaneous Localization and Mapping)
- 📸 Pi Camera for visual object detection
- 🗺️ Real-time mapping and autonomous navigation

#### **Visual Intelligence**
- 🤖 YOLO/MobileNet models for object detection
- 🔥 Hazard detection (fire, obstacles, humans)
- 📦 Label and package recognition for warehouse operations

#### **Outdoor Navigation**
- 🛰️ NEO-6M GPS module for coordinate-based travel
- 🧭 Digital compass for heading correction
- 🌐 Integration of GPS waypoint navigation

#### **Industrial Mechanics**
- 💪 Upgrade to 35kg payload capacity
- 🚗 Ackermann steering with 60kg servo
- ⚡ 43A motor drivers for high-torque applications
- 🔋 Extended battery life (8+ hours)

#### **Fleet Management**
- 📡 Multi-robot coordination
- ☁️ Cloud-based monitoring dashboard
- 📊 Analytics and performance tracking

### Phase 3: AI & IoT Evolution

#### **Artificial Intelligence (TinyML)**
- 🧠 Edge AI for real-time decision making
- 🔧 Predictive maintenance via vibration/current analysis
- 🎯 Autonomous task planning and optimization

#### **Enhanced Connectivity**
- 📶 Migration from local Wi-Fi to 5G/LoRaWAN
- ☁️ Full cloud integration for remote operations
- 🌍 Multi-site deployment capabilities

### Industrial Applications

#### **Hazardous Environments**
- ☢️ Chemical plant monitoring
- 🔥 Fire detection and response
- 🏭 High-temperature area inspection

#### **Warehouse Automation**
- 📦 Inventory tracking and transport
- 🏷️ Automated picking assistance
- 📍 Guided lane following

#### **Security & Surveillance**
- 🎥 Autonomous patrol routes
- 🚨 Anomaly detection and alerting
- 📹 24/7 monitoring coverage

---

## 👨‍💻 Team

**Project Team (SSIP)**
- **Pradeep Kalsaria** - Lead Developer & Presenter
- **Jamunesh Sheta** - Co-Developer & Presenter

**Mentor**
- **Ayush Dodia Sir** - Project Mentor

## 🙏 Acknowledgments

**Project Goal**: Democratize access to robotics technology in hazardous environments by creating cost-effective alternatives to expensive commercial AMRs.

### Special Thanks
- **Ayush Dodia Sir** - Project Mentor for guidance and support
- Arduino Community - For open-source tools and libraries
- ESP8266 Community - For IoT capabilities and documentation
- All contributors and testers - For feedback and improvements

### Mission Statement
*"We have successfully demonstrated a robust, scalable architecture. Our goal is to democratize access to robotics in hazardous environments."* - Nexus Rover Team

## 📸 Gallery

*(Add photos/videos of your rover in action!)*

---

**⭐ If you find this project useful, please consider giving it a star!**

## � Documentation

| Document | Description |
|----------|-------------|
| [Quick Start Guide](docs/QUICK_START.md) | Get running in 30 minutes |
| [Hardware Guide](docs/HARDWARE.md) | Components, wiring, assembly |
| [System Architecture](docs/ARCHITECTURE.md) | Technical design details |
| [Market Analysis](docs/MARKET_ANALYSIS.md) | Competitive positioning |
| [Troubleshooting](docs/TROUBLESHOOTING.md) | Common issues & fixes |
| [Contributing](docs/CONTRIBUTING.md) | How to contribute |
| [GitHub Setup](docs/GITHUB_SETUP.md) | Publishing guide |

## 🔗 Useful Links

- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [ESP8266 Documentation](https://arduino-esp8266.readthedocs.io/)
- [L298N Motor Driver Guide](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/)
- [HC-SR04 Ultrasonic Sensor](https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/)
- [SSIP Initiative](https://www.ssipgujarat.in/)

---

**Built with ❤️ for robotics enthusiasts**

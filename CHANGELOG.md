# Changelog

All notable changes to the Nexus Rover project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Gyroscope integration for improved stability
- ESP32-CAM support for FPV streaming
- Mobile app (Android/iOS)
- Voice control integration
- Speed encoders for precise distance tracking
- OLED display for on-rover status

---

## [1.0.0] - 2025-12-27

### Initial Release - SSIP Project

**Project Context**: Developed as part of Student Startup & Innovation Policy (SSIP) initiative to demonstrate cost-effective autonomous robotics for industrial and hazardous applications.

**Team**: Pradeep Kalsaria, Jamunesh Sheta | **Mentor**: Ayush Dodia Sir

#### Added - Arduino Controller
- **6 Driving Modes:**
  - PARK mode
  - LINE FOLLOW with dual IR sensors
  - FOLLOW ME using ultrasonic sensor
  - EDGE DETECT for table-top safety
  - MANUAL WiFi control
  - AUTO PILOT with 90° obstacle avoidance
  
- **Motor Control:**
  - L298N dual motor driver support
  - PWM speed control (0-255)
  - Adaptive turning based on movement state
  - Precision 90° calibrated turns
  
- **Sensors:**
  - HC-SR04 ultrasonic distance measurement
  - Dual IR sensors for line/edge detection
  - Voltage monitoring for battery level
  
- **Features:**
  - Musical horn with multi-tone melody
  - Emergency stop functionality
  - Toggle-based movement controls
  - Auto-stop for 90° turns
  - Serial debugging output
  - Telemetry transmission to ESP8266

#### Added - ESP8266 WiFi Module
- **Web Server:**
  - Async web server for better performance
  - WebSocket real-time communication
  - Responsive web interface
  
- **User Interface:**
  - Beautiful gradient design with starfield animation
  - Mobile-optimized touch controls
  - 6-mode selector buttons
  - Steering wheel visualization
  - Pedal-style forward/reverse controls
  - 90° turn buttons
  - Real-time battery voltage display
  - Emergency stop button
  - Press-and-hold horn button
  
- **Communication:**
  - Bidirectional Arduino communication via SoftwareSerial
  - Command parsing and forwarding
  - Telemetry relay to web clients

#### Documentation
- Comprehensive README.md with features and setup
- HARDWARE.md with component specs and wiring
- TROUBLESHOOTING.md with common issues and solutions
- CONTRIBUTING.md with development guidelines
- QUICK_START.md for fast setup
- MIT License

#### Configuration
- .gitignore for Arduino projects
- Tunable parameters for speeds, distances, and calibration
- WiFi credential configuration

---

## Version History

### Key Milestones

**v1.0.0** - SSIP Presentation & Public Release
- Complete feature set for autonomous and manual control
- Production-ready code
- Full documentation
- Market analysis and competitive positioning
- Validated sub-$50 proof-of-concept
- Presented at SSIP with roadmap to $800 industrial prototype

**v0.9.0** - Beta release (internal)
- All features implemented
- Testing and bug fixes
- Documentation drafted

**v0.5.0** - Alpha release (internal)
- Basic motor control
- Manual mode functional
- ESP8266 web interface prototype

**v0.1.0** - Proof of concept
- Simple motor test
- Basic Arduino-ESP communication

---

## Update Notes

### How to Update Your Rover

When new versions are released:

1. **Backup your configuration:**
   - Save WiFi credentials
   - Note any custom calibration values
   - Document code modifications

2. **Download latest release:**
   ```bash
   git pull origin main
   ```

3. **Review CHANGELOG:**
   - Check for breaking changes
   - Note new features
   - Read migration notes

4. **Update configuration:**
   - Re-apply WiFi credentials
   - Re-calibrate if needed
   - Test all functions

5. **Upload new code:**
   - Upload to Arduino
   - Upload to ESP8266
   - Verify functionality

---

## Breaking Changes

None yet! This is the first release.

Future breaking changes will be clearly marked with ⚠️ WARNING.

---

## Support

For issues with specific versions:
- Check the corresponding documentation
- Search closed issues for solutions
- Open new issue with version number

---

**Stay updated! Watch the repository for new releases. ⭐**

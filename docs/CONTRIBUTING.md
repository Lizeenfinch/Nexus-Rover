# 🤝 Contributing to Nexus Rover

First off, thank you for considering contributing to Nexus Rover! It's people like you that make this project better for everyone.

## 📋 Table of Contents
- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)

---

## Code of Conduct

This project and everyone participating in it is governed by respect and collaboration. By participating, you are expected to uphold this standard.

**Our Standards:**
- ✅ Be respectful and inclusive
- ✅ Welcome newcomers and help them learn
- ✅ Focus on constructive feedback
- ✅ Accept differing viewpoints gracefully
- ❌ No harassment, discrimination, or unprofessional conduct

---

## How Can I Contribute?

### 🐛 Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates.

**When submitting a bug report, include:**
- **Clear title** - Concise description of the issue
- **Environment** - Hardware used, Arduino/ESP8266 board version
- **Steps to reproduce** - Detailed steps to trigger the bug
- **Expected behavior** - What should happen
- **Actual behavior** - What actually happens
- **Serial Monitor output** - Copy relevant log messages
- **Photos/Videos** - If applicable (especially for hardware issues)
- **Code modifications** - Any changes you made to the original code

**Bug Report Template:**
```markdown
### Description
[Clear description of the bug]

### Hardware
- Arduino: Uno / Nano / Mega
- ESP8266: NodeMCU / Other
- Motor Driver: L298N / Other
- Battery: [Voltage and type]

### Steps to Reproduce
1. Step one
2. Step two
3. Step three

### Expected Behavior
[What should happen]

### Actual Behavior
[What actually happens]

### Serial Output
```
[Paste relevant output]
```

### Additional Context
[Screenshots, videos, or other details]
```

---

### 💡 Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues.

**When suggesting enhancements:**
- **Use clear title** - "Add gyroscope stabilization" not "Make it better"
- **Describe current limitation** - What problem does this solve?
- **Describe proposed solution** - How would this work?
- **Consider alternatives** - Are there other approaches?
- **Implementation complexity** - Simple tweak or major overhaul?

**Enhancement Template:**
```markdown
### Problem
[What limitation or problem exists currently?]

### Proposed Solution
[How would this enhancement work?]

### Benefits
- Benefit 1
- Benefit 2

### Implementation Notes
[Technical details, dependencies, challenges]

### Alternatives Considered
[Other approaches you thought about]
```

---

### 🔧 Hardware Contributions

**New Hardware Modules:**
- Create wiring diagram
- Update HARDWARE.md with specifications
- Provide code example for integration
- Test thoroughly before submitting

**Chassis Designs:**
- Share 3D models (STL, STEP files)
- Include assembly instructions
- List required hardware (screws, standoffs)
- Photos of finished build

---

### 📝 Documentation Improvements

Documentation is just as important as code!

**Areas for improvement:**
- Fixing typos or unclear explanations
- Adding diagrams or photos
- Creating video tutorials
- Translating to other languages
- Adding troubleshooting tips
- Writing example projects

**To contribute docs:**
1. Fork the repository
2. Edit markdown files in `/docs` or main README
3. Submit pull request with clear description

---

### 💻 Code Contributions

See sections below for detailed guidelines.

---

## Development Setup

### Prerequisites

**Software:**
- Arduino IDE 1.8.13+ or Arduino IDE 2.0+
- Git for version control
- Code editor (VSCode recommended for Arduino extension)

**Hardware (for testing):**
- Minimum: Arduino + ESP8266 + L298N + motors
- Full setup preferred for comprehensive testing

### Getting the Code

```bash
# Fork the repository on GitHub, then:
git clone https://github.com/YOUR_USERNAME/nexus-rover.git
cd nexus-rover

# Add upstream remote
git remote add upstream https://github.com/ORIGINAL_OWNER/nexus-rover.git
```

### Creating a Branch

```bash
# Create feature branch from main
git checkout -b feature/your-feature-name

# For bug fixes
git checkout -b fix/bug-description
```

---

## Coding Standards

### Arduino Code Style

**Formatting:**
- Indentation: 2 spaces (no tabs)
- Braces: K&R style (opening brace on same line)
- Line length: Aim for 80-100 characters

**Naming Conventions:**
```cpp
// Constants: ALL_CAPS with underscores
const int MAX_SPEED = 255;
const int SONAR_TRIG_PIN = 4;

// Variables: camelCase
int currentSpeed = 0;
bool isMovingForward = false;

// Functions: camelCase with descriptive names
void setMotorSpeed(int speed) { }
int getSonarDistance() { }

// Enums: PascalCase for type, CAPS for values
enum DriveMode {
  PARK,
  LINE_FOLLOW,
  MANUAL
};
```

**Comments:**
```cpp
// Single-line comments for brief explanations
int speed = 100; // Maximum safe speed

/* Multi-line comments for complex logic
 * Explain WHY not WHAT when possible
 * Code should be self-documenting
 */

// Section headers
//======================================================================
// --- MOTOR CONTROL ---
//======================================================================
```

**Functions:**
- One function, one purpose
- Keep functions short (<50 lines ideally)
- Use descriptive names (no single letters except loop counters)
- Add comments for complex algorithms

**Example:**
```cpp
/**
 * Calculate distance using HC-SR04 ultrasonic sensor
 * @return Distance in centimeters, or 0 if timeout
 */
long getSonarDistance() {
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
  
  long duration = pulseIn(SONAR_ECHO_PIN, HIGH, 25000);
  return duration * 0.034 / 2;
}
```

---

### ESP8266 Code Style

**Same conventions as Arduino, plus:**

**HTML/CSS/JavaScript in PROGMEM:**
```cpp
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
...
</html>
)rawliteral";
```

**Asynchronous Callbacks:**
```cpp
// Use lambda functions for WebSocket events
ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, 
              AwsEventType type, void *arg, uint8_t *data, size_t len) {
  // Handler code
});
```

---

## Testing Guidelines

### Before Submitting Code

**Test Checklist:**
- [ ] Code compiles without errors on Arduino
- [ ] Code compiles without errors on ESP8266
- [ ] All existing features still work
- [ ] New feature works as expected
- [ ] Serial Monitor shows no unexpected errors
- [ ] Motors respond correctly
- [ ] Sensors function properly
- [ ] Web interface works (if ESP code changed)
- [ ] Battery monitoring displays correctly
- [ ] Code works with low battery (7V test)

### Testing New Features

1. **Unit Testing** - Test feature in isolation
2. **Integration Testing** - Test with other features
3. **Edge Cases** - Test boundary conditions
4. **Error Handling** - Test failure scenarios

**Example: Testing New Sensor**
```cpp
// 1. Test sensor alone
void testNewSensor() {
  int reading = analogRead(NEW_SENSOR_PIN);
  Serial.println("Sensor: " + String(reading));
}

// 2. Test with existing code
// 3. Test when sensor fails/disconnects
// 4. Test extreme values
```

---

## Commit Guidelines

### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only
- `style`: Code formatting (no logic change)
- `refactor`: Code restructuring (no feature change)
- `test`: Adding tests
- `chore`: Build process, dependencies

**Subject:**
- Use imperative mood ("Add feature" not "Added feature")
- No period at the end
- Limit to 50 characters

**Body:**
- Wrap at 72 characters
- Explain WHAT and WHY, not HOW
- Separate from subject with blank line

**Examples:**

```
feat: Add gyroscope stabilization support

Integrate MPU6050 sensor for improved straight-line driving
and turn accuracy. Includes calibration routine on startup.

Closes #42
```

```
fix: Correct motor direction in AUTO_PILOT mode

Left motor was reversed during obstacle avoidance turns.
Swapped IN1/IN2 logic for proper rotation.

Fixes #38
```

```
docs: Add wiring diagram for ESP32-CAM integration

Include pinout, power requirements, and code example
for camera streaming feature.
```

---

## Pull Request Process

### Before Creating PR

1. **Update your branch** with latest upstream changes:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Test thoroughly** - Run all tests mentioned above

3. **Update documentation** - If you added features, update:
   - README.md
   - HARDWARE.md (if hardware-related)
   - TROUBLESHOOTING.md (if fixing common issue)

4. **Check code style** - Ensure formatting is consistent

### Creating the Pull Request

1. **Push to your fork:**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Open PR on GitHub** with clear description:

**PR Template:**
```markdown
## Description
[Clear description of what this PR does]

## Type of Change
- [ ] Bug fix (non-breaking change fixing an issue)
- [ ] New feature (non-breaking change adding functionality)
- [ ] Breaking change (fix or feature causing existing functionality to change)
- [ ] Documentation update

## Testing
- [ ] Code compiles on Arduino
- [ ] Code compiles on ESP8266
- [ ] Tested on hardware
- [ ] All existing features work
- [ ] New feature works as expected

## Screenshots/Videos
[If applicable, add visuals]

## Related Issues
Closes #[issue number]

## Additional Notes
[Any other relevant information]
```

### PR Review Process

1. **Automated checks** - Ensure compilation succeeds
2. **Code review** - Maintainer reviews changes
3. **Feedback** - Address any comments or requests
4. **Approval** - Once approved, PR will be merged

**Tips:**
- Be responsive to feedback
- Don't take criticism personally
- Ask questions if unclear
- Be patient - reviews take time

---

## Recognition

Contributors will be:
- Listed in README.md acknowledgments
- Credited in release notes
- Given credit in code comments (for major features)

---

## Questions?

- **General questions:** Open a GitHub Discussion
- **Bugs:** Open an Issue
- **Security vulnerabilities:** Email maintainer directly (don't open public issue)

---

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for making Nexus Rover better! 🚀**

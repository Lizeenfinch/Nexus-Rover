#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

// --- UPDATE WITH YOUR WIFI CREDENTIALS ---
const char* ssid = "Pradeep's S23 Ultra";
const char* password = "pradeep2910";

// --- ARDUINO COMMUNICATION ---
const int ARDUINO_RX = D6;
const int ARDUINO_TX = D5;
SoftwareSerial arduino(ARDUINO_RX, ARDUINO_TX);

// --- WEB SERVER & WEBSOCKET SETUP ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// --- HTML, CSS, AND JAVASCRIPT FOR THE USER INTERFACE ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>Nexus Control</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;700&display=swap');
        :root {
            --bg-color: #0a0a0a;
            --surface-color: #1a1a1a;
            --primary-color: #00d4ff;
            --on-surface: #ffffff;
            --on-primary: #000000;
            --accent-green: #00ff88;
            --accent-red: #ff4444;
            --accent-yellow: #ffcc00;
        }
        body{font-family:'Inter',sans-serif;background:linear-gradient(135deg,#0a0a0a 0%,#1a1a2e 50%,#16213e 100%);color:var(--on-surface);margin:0;padding:10px;min-height:100vh;-webkit-tap-highlight-color:transparent;overflow-x:hidden;touch-action:manipulation;position:relative}.starfield{position:fixed;top:0;left:0;width:100%;height:100%;pointer-events:none;z-index:-1;overflow:hidden}.star{position:absolute;background:radial-gradient(circle,#fff 0%,transparent 70%);border-radius:50%;opacity:0;animation-timing-function:linear;animation-iteration-count:infinite;box-shadow:0 0 4px rgba(255,255,255,.4)}.star.small{width:2px;height:2px;animation-name:fallingStar;animation-duration:4s;opacity:.6;box-shadow:0 0 5px #fff}.star.medium{width:3px;height:3px;animation-name:fallingStarMedium;animation-duration:5s;opacity:.8;box-shadow:0 0 8px #fff,0 0 12px rgba(255,255,255,.5)}.star.large{width:4px;height:4px;animation-name:fallingStarLarge;animation-duration:6s;opacity:.9;box-shadow:0 0 12px #00d4ff,0 0 25px #00d4ff,0 0 40px #00d4ff}.star.shooting{width:6px;height:6px;background:linear-gradient(45deg,#fff,#00d4ff,#00ff88);animation-name:shootingStar;animation-duration:2.5s;opacity:1;box-shadow:0 0 20px #00d4ff,0 0 40px #00d4ff,0 0 60px #00d4ff}@keyframes fallingStar{0%{opacity:0;transform:translateY(-100vh) translateX(0)}10%{opacity:.8}90%{opacity:.8}100%{opacity:0;transform:translateY(100vh) translateX(-50px)}}@keyframes fallingStarMedium{0%{opacity:0;transform:translateY(-100vh) translateX(0) scale(1)}15%{opacity:1}85%{opacity:1}100%{opacity:0;transform:translateY(100vh) translateX(-75px) scale(.5)}}@keyframes fallingStarLarge{0%{opacity:0;transform:translateY(-100vh) translateX(0) scale(1)}20%{opacity:1}80%{opacity:1}100%{opacity:0;transform:translateY(100vh) translateX(-100px) scale(.3)}}@keyframes shootingStar{0%{opacity:0;transform:translateY(-50px) translateX(-50px) scale(0)}10%{opacity:1;transform:translateY(0) translateX(0) scale(1)}90%{opacity:1}100%{opacity:0;transform:translateY(100vh) translateX(-200px) scale(.2)}}.container{width:100%;max-width:1200px;margin:0 auto;display:flex;flex-direction:column;height:calc(100vh - 20px);position:relative;z-index:1}.header{display:flex;justify-content:space-between;align-items:center;padding:15px 20px;background:rgba(26,26,26,.9);border-radius:15px;margin-bottom:15px;backdrop-filter:blur(10px);border:1px solid rgba(255,255,255,.1)}.title{font-size:1.5em;font-weight:700;color:var(--primary-color)}.battery-indicator{display:flex;align-items:center;gap:10px;background:rgba(0,255,136,.1);padding:8px 15px;border-radius:25px;border:1px solid var(--accent-green)}.battery-icon{font-size:1.2em}.voltage-value{font-weight:600;color:var(--accent-green)}.modes-section{margin-bottom:20px}.section-title{font-size:.9em;color:#aaa;margin-bottom:10px;text-transform:uppercase;letter-spacing:1px}.modes-bar{display:grid;grid-template-columns:repeat(6,1fr);gap:8px;padding:15px;background:rgba(26,26,26,.6);border-radius:12px;backdrop-filter:blur(10px)}.mode-btn{padding:12px 8px;border:none;border-radius:8px;font-size:.8em;font-weight:600;cursor:pointer;transition:all .3s ease;background:rgba(255,255,255,.1);color:var(--on-surface);border:1px solid transparent}.mode-btn.active{background:var(--primary-color);color:var(--on-primary);border-color:var(--primary-color);transform:translateY(-2px);box-shadow:0 5px 15px rgba(0,212,255,.4)}.control-interface{flex:1;display:grid;grid-template-columns:1fr 1fr;gap:20px;padding:0 10px}.steering-section{display:flex;flex-direction:column;align-items:center;justify-content:center;background:rgba(26,26,26,.8);border-radius:20px;padding:30px;backdrop-filter:blur(15px);border:1px solid rgba(255,255,255,.1)}.steering-title{color:#ccc;margin-bottom:20px;font-size:1.1em;font-weight:600}.steering-wheel{width:200px;height:200px;border:8px solid #444;border-radius:50%;position:relative;background:linear-gradient(135deg,#2a2a2a,#1a1a1a);display:flex;align-items:center;justify-content:center;box-shadow:inset 0 0 30px rgba(0,0,0,.5),0 10px 30px rgba(0,0,0,.3);transition:transform .2s ease}.steering-wheel::before{content:'';width:60px;height:60px;background:#333;border-radius:50%;position:absolute;border:3px solid #555}.steering-wheel::after{content:'⚡';font-size:1.5em;color:var(--primary-color);position:absolute;z-index:1}.turn-buttons{display:flex;gap:20px;margin-top:25px}.turn-btn{padding:15px 25px;border:none;border-radius:12px;font-size:1em;font-weight:600;cursor:pointer;transition:all .2s ease;color:#fff;display:flex;align-items:center;gap:8px;min-width:120px;justify-content:center}.turn-left{background:linear-gradient(135deg,#ff6b6b,#ee5a52)}.turn-right{background:linear-gradient(135deg,#4ecdc4,#44a08d)}.turn-btn:active{transform:scale(.95)}.pedals-section{display:flex;flex-direction:column;background:rgba(26,26,26,.8);border-radius:20px;padding:30px;backdrop-filter:blur(15px);border:1px solid rgba(255,255,255,.1)}.pedals-title{color:#ccc;margin-bottom:25px;font-size:1.1em;font-weight:600;text-align:center}.pedals-container{display:flex;flex-direction:column;gap:20px;flex:1}.pedal{flex:1;border:none;border-radius:15px;font-size:1.2em;font-weight:700;cursor:pointer;transition:all .2s ease;color:#fff;display:flex;align-items:center;justify-content:center;gap:12px;min-height:80px;border:3px solid transparent;position:relative;overflow:hidden}.pedal::before{content:'';position:absolute;top:0;left:0;right:0;bottom:0;opacity:0;transition:opacity .2s ease}.accelerator{background:linear-gradient(135deg,#00ff88,#00cc6a)}.accelerator:active::before{background:rgba(255,255,255,.2);opacity:1}.brake{background:linear-gradient(135deg,#ff4444,#cc3333)}.brake:active::before{background:rgba(255,255,255,.2);opacity:1}.pedal:active{transform:scale(.98) translateY(2px);box-shadow:inset 0 5px 15px rgba(0,0,0,.3)}.speed-display{background:rgba(0,0,0,.6);padding:15px;border-radius:12px;margin-top:20px;text-align:center}.speed-label{font-size:.8em;color:#888;margin-bottom:5px}.speed-value{font-size:2em;font-weight:700;color:var(--primary-color);font-family:'Courier New',monospace}.bottom-controls{display:flex;gap:15px;justify-content:center;padding:20px}.utility-btn{padding:15px 30px;border:none;border-radius:25px;font-size:1em;font-weight:600;cursor:pointer;transition:all .2s ease;color:#fff;display:flex;align-items:center;gap:8px}.stop-btn{background:linear-gradient(135deg,#ff4444,#cc3333)}.horn-btn{background:linear-gradient(135deg,#ffcc00,#ff9900);color:#000}.utility-btn:active{transform:scale(.95)}@media (max-width:768px){.control-interface{grid-template-columns:1fr;gap:15px}.modes-bar{grid-template-columns:repeat(3,1fr)}.steering-wheel{width:150px;height:150px}.pedals-container{flex-direction:row}}
    </style>
</head>
<body>
    <div class="starfield" id="starfield"></div>
    <div class="container">
        <div class="header">
            <div class="title">🤖 Nexus Drive</div>
            <div class="battery-indicator">
                <span class="battery-icon">🔋</span>
                <span class="voltage-value" id="voltage-display">--.-V</span>
            </div>
        </div>
        <div class="modes-section">
            <div class="section-title">Drive Mode</div>
            <div class="modes-bar">
                <button class="mode-btn active" onclick="setMode('0')">PARK</button>
                <button class="mode-btn" onclick="setMode('1')">LINE</button>
                <button class="mode-btn" onclick="setMode('2')">FOLLOW</button>
                <button class="mode-btn" onclick="setMode('3')">EDGE</button>
                <button class="mode-btn" onclick="setMode('4')">MANUAL</button>
                <button class="mode-btn" onclick="setMode('5')">AUTO</button>
            </div>
        </div>
        <div class="control-interface">
            <div class="steering-section">
                <div class="steering-title">🎮 STEERING</div>
                <div class="steering-wheel" id="steering-wheel"></div>
                <div class="turn-buttons">
                    <button class="turn-btn turn-left" id="turn-left"><span>◀</span> LEFT</button>
                    <button class="turn-btn turn-right" id="turn-right"><span>▶</span> RIGHT</button>
                </div>
            </div>
            <div class="pedals-section">
                <div class="pedals-title">🦶 PEDALS</div>
                <div class="pedals-container">
                    <button class="pedal accelerator" id="accelerator"><span>⚡</span> FORWARD</button>
                    <button class="pedal brake" id="brake"><span>⬇</span> REVERSE</button>
                </div>
                <div class="speed-display">
                    <div class="speed-label">CURRENT SPEED</div>
                    <div class="speed-value" id="speed-value">0</div>
                </div>
            </div>
        </div>
        <div class="bottom-controls">
            <button class="utility-btn stop-btn" id="emergency-stop"><span>⏹</span> E-STOP</button>
            <button class="utility-btn horn-btn" id="horn"><span>📯</span> HORN</button>
        </div>
    </div>
    <script>
        let websocket;
        let currentMode = '0';
        let currentSpeed = 0;
        let commandInterval = null;
        let isAccelerating = false;
        let isBraking = false;
        let isTurningLeft = false;
        let isTurningRight = false;
        let isHornPressed = false;

        function createStar() {
            const star = document.createElement('div');
            star.className = 'star';
            const types = ['small', 'medium', 'large', 'shooting'];
            const weights = [40, 35, 20, 5]; 
            const randomType = weightedRandom(types, weights);
            star.classList.add(randomType);
            star.style.left = Math.random() * window.innerWidth + 'px';
            star.style.animationDelay = Math.random() * 2 + 's';
            document.getElementById('starfield').appendChild(star);
            
            const duration = randomType === 'shooting' ? 2500 : 
                             randomType === 'large' ? 6000 : 
                             randomType === 'medium' ? 5000 : 4000;
            
            setTimeout(() => {
                if (star.parentNode) star.parentNode.removeChild(star);
            }, duration + 2000);
        }
        function weightedRandom(items, weights) {
            const cumulativeWeights = [];
            for (let i = 0; i < weights.length; i++) {
                cumulativeWeights[i] = weights[i] + (cumulativeWeights[i - 1] || 0);
            }
            const maxCumulativeWeight = cumulativeWeights[cumulativeWeights.length - 1];
            const randomNumber = maxCumulativeWeight * Math.random();
            for (let itemIndex = 0; itemIndex < items.length; itemIndex++) {
                if (cumulativeWeights[itemIndex] >= randomNumber) return items[itemIndex];
            }
        }
        setInterval(createStar, 150);
        for (let i = 0; i < 40; i++) {
            setTimeout(createStar, Math.random() * 2000);
        }

        function initWebSocket() {
            websocket = new WebSocket(`ws://${window.location.hostname}/ws`);
            websocket.onopen = () => { console.log('WebSocket connected!'); showFeedback('🎮 Connected!'); };
            websocket.onclose = () => { console.log('WebSocket disconnected!'); showFeedback('🔌 Disconnected!'); setTimeout(initWebSocket, 2000); };
            websocket.onmessage = (event) => {
                if (event.data.startsWith('V:')) {
                    const voltage = event.data.substring(2);
                    document.getElementById('voltage-display').textContent = `${voltage}V`;
                }
            };
        }

        function setMode(mode) {
            if (currentMode === mode) return;
            currentMode = mode;
            document.querySelectorAll('.mode-btn').forEach(btn => btn.classList.remove('active'));
            document.querySelectorAll('.mode-btn')[parseInt(mode)].classList.add('active');
            showFeedback(`Mode: ${['PARK', 'LINE', 'FOLLOW', 'EDGE', 'MANUAL', 'AUTO'][parseInt(mode)]}`);
            sendCommand(`M${mode}`);
        }

        function sendCommand(cmd) {
            if (websocket && websocket.readyState === WebSocket.OPEN) {
                websocket.send(cmd);
            }
        }

        function updateSteeringWheel() {
            const wheel = document.getElementById('steering-wheel');
            let rotation = 0;
            if (isTurningLeft) rotation = -30;
            if (isTurningRight) rotation = 30;
            wheel.style.transform = `rotate(${rotation}deg)`;
        }

        function updateSpeed() {
            const speedDisplay = document.getElementById('speed-value');
            if (isAccelerating && currentSpeed < 255) {
                currentSpeed = Math.min(currentSpeed + 5, 255);
            } else if (isBraking && currentSpeed > -255) {
                currentSpeed = Math.max(currentSpeed - 5, -255);
            } else if (!isAccelerating && !isBraking) {
                if (currentSpeed > 0) currentSpeed = Math.max(currentSpeed - 3, 0);
                else if (currentSpeed < 0) currentSpeed = Math.min(currentSpeed + 3, 0);
            }
            speedDisplay.textContent = Math.abs(Math.round(currentSpeed));
        }
        
        function showFeedback(message) {
            const existingFeedback = document.querySelector('.feedback-toast');
            if (existingFeedback) existingFeedback.remove();
            const feedback = document.createElement('div');
            feedback.className = 'feedback-toast';
            feedback.style.cssText = `
                position: fixed; top: 20px; left: 50%;
                transform: translateX(-50%);
                background: linear-gradient(135deg, #00d4ff, #0099cc);
                color: white; padding: 12px 20px; border-radius: 25px;
                font-weight: 600; z-index: 1001;
                box-shadow: 0 5px 20px rgba(0, 212, 255, 0.4);
                animation: slideDown 2s ease-in-out;
            `;
            feedback.textContent = message;
            document.body.appendChild(feedback);
            setTimeout(() => feedback.remove(), 2000);
        }
        const style = document.createElement('style');
        style.textContent = `@keyframes slideDown {
            0% { opacity: 0; transform: translateX(-50%) translateY(-20px); }
            20% { opacity: 1; transform: translateX(-50%) translateY(0); }
            80% { opacity: 1; transform: translateX(-50%) translateY(0); }
            100% { opacity: 0; transform: translateX(-50%) translateY(10px); }
        }`;
        document.head.appendChild(style);
        
        function startSending(command) {
            if (currentMode !== '4' || commandInterval) return;

            if (command === 'F') isAccelerating = true;
            if (command === 'B') isBraking = true;
            if (command === 'L') { isTurningLeft = true; updateSteeringWheel(); }
            if (command === 'R') { isTurningRight = true; updateSteeringWheel(); }

            sendCommand(command);
            commandInterval = setInterval(() => sendCommand(command), 150);
        }

        function stopSending() {
            if (!commandInterval) return;

            clearInterval(commandInterval);
            commandInterval = null;

            isAccelerating = isBraking = isTurningLeft = isTurningRight = false;
            updateSteeringWheel();
            
            sendCommand('S');
        }

        function addTouchAndMouseListeners(element, command) {
            element.addEventListener('mousedown', () => startSending(command));
            element.addEventListener('touchstart', (e) => { e.preventDefault(); startSending(command); });

            element.addEventListener('mouseup', stopSending);
            element.addEventListener('mouseleave', stopSending);
            element.addEventListener('touchend', stopSending);
        }
        
        // MUSICAL HORN FUNCTIONS
        function startHorn() {
            if (!isHornPressed) {
                isHornPressed = true;
                sendCommand('H');
                console.log('Horn pressed - sent H');
            }
        }
        
        function stopHorn() {
            if (isHornPressed) {
                isHornPressed = false;
                sendCommand('h');
                console.log('Horn released - sent h');
            }
        }
        
        addTouchAndMouseListeners(document.getElementById('accelerator'), 'F');
        addTouchAndMouseListeners(document.getElementById('brake'), 'B');
        addTouchAndMouseListeners(document.getElementById('turn-left'), 'L');
        addTouchAndMouseListeners(document.getElementById('turn-right'), 'R');

        document.getElementById('emergency-stop').addEventListener('click', () => {
            currentSpeed = 0;
            stopSending();
        });
        
        // Horn button with press and release
        const hornButton = document.getElementById('horn');
        hornButton.addEventListener('mousedown', startHorn);
        hornButton.addEventListener('touchstart', (e) => { e.preventDefault(); startHorn(); });
        hornButton.addEventListener('mouseup', stopHorn);
        hornButton.addEventListener('mouseleave', stopHorn);
        hornButton.addEventListener('touchend', stopHorn);
        
        window.addEventListener('load', initWebSocket);
        setInterval(updateSpeed, 50);
    </script>
</body>
</html>
)rawliteral";

// --- WEBSOCKET EVENT HANDLER ---
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            AwsFrameInfo *info;
            info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = 0;
                String command = (char*)data;
                
                arduino.print(command);

                Serial.printf("Received from web, sent to Arduino: %s\n", command.c_str());
            }
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

// --- SETUP ---
void setup() {
    Serial.begin(115200); 
    arduino.begin(115200);
    
    Serial.println("ESP8266 Starting...");
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.begin();
    Serial.println("Web server started");
}

// --- MAIN LOOP ---
void loop() {
    ws.cleanupClients();

    if (arduino.available()) {
        String telemetry = arduino.readStringUntil('\n');
        telemetry.trim();
        if (telemetry.length() > 0) {
            ws.textAll(telemetry);
        }
    }
}
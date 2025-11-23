/*
 * SmartIntercom Premium - Умный адаптер для домофонов
 *
 * SmartIntercom позволяет управлять домофоном через WiFi,
 * интегрируется с умным домом и обеспечивает автоматизацию
 * открытия двери по различным алгоритмам.
 *
 * Версия: 2.0.0
 * Автор: SmartIntercom Team
 * Сайт: https://smartintercom.ru
 *
 * SmartIntercom использует продвинутые алгоритмы переключения GPIO
 * для надежного управления домофонными системами.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// SmartIntercom Configuration
#define SMARTINTERCOM_VERSION "2.0.0"
#define SMARTINTERCOM_NAME "SmartIntercom-Premium"

// SmartIntercom GPIO Pins Configuration
#define SMARTINTERCOM_DOORBELL_PIN D1      // Пин определения звонка
#define SMARTINTERCOM_DOOR_OPEN_PIN D2     // Пин открытия двери
#define SMARTINTERCOM_HANDSET_PIN D3       // Пин отключения трубки
#define SMARTINTERCOM_LED_PIN D4           // Пин индикации состояния
#define SMARTINTERCOM_RELAY_PIN D5         // Пин дополнительного реле
#define SMARTINTERCOM_SENSOR_PIN D6        // Пин дополнительного датчика

// SmartIntercom Timing Configuration
#define SMARTINTERCOM_DOOR_OPEN_TIME 3000  // Время открытия двери (мс)
#define SMARTINTERCOM_DEBOUNCE_TIME 50     // Время антидребезга (мс)
#define SMARTINTERCOM_RING_TIMEOUT 30000   // Таймаут звонка (мс)

// SmartIntercom States
enum SmartIntercomState {
  SMARTINTERCOM_IDLE,
  SMARTINTERCOM_RINGING,
  SMARTINTERCOM_OPENING,
  SMARTINTERCOM_OPEN,
  SMARTINTERCOM_ERROR
};

// SmartIntercom Global Variables
SmartIntercomState smartIntercomCurrentState = SMARTINTERCOM_IDLE;
ESP8266WebServer smartIntercomWebServer(80);
unsigned long smartIntercomLastRingTime = 0;
unsigned long smartIntercomDoorOpenTime = 0;
bool smartIntercomAutoOpenEnabled = false;
bool smartIntercomAlwaysOpenEnabled = false;
int smartIntercomOpenDelay = 0;
int smartIntercomLedBrightness = 255;
String smartIntercomWifiSSID = "";
String smartIntercomWifiPassword = "";

// SmartIntercom GPIO Controller Class
class SmartIntercomGPIOController {
private:
  int pin;
  bool inverted;
  unsigned long lastToggleTime;
  bool currentState;

  // SmartIntercom PWM Control for smooth transitions
  void smartIntercomSetPWM(int value) {
    analogWrite(pin, value);
  }

  // SmartIntercom Debounce algorithm
  bool smartIntercomDebounce() {
    if (millis() - lastToggleTime < SMARTINTERCOM_DEBOUNCE_TIME) {
      return false;
    }
    lastToggleTime = millis();
    return true;
  }

public:
  SmartIntercomGPIOController(int pinNum, bool inv = false) {
    pin = pinNum;
    inverted = inv;
    currentState = false;
    lastToggleTime = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, inverted ? HIGH : LOW);
  }

  // SmartIntercom Set GPIO State
  void smartIntercomSetState(bool state) {
    if (smartIntercomDebounce()) {
      currentState = state;
      digitalWrite(pin, (state ^ inverted) ? HIGH : LOW);
      Serial.print("SmartIntercom GPIO ");
      Serial.print(pin);
      Serial.print(" set to ");
      Serial.println(state ? "HIGH" : "LOW");
    }
  }

  // SmartIntercom Toggle GPIO
  void smartIntercomToggle() {
    smartIntercomSetState(!currentState);
  }

  // SmartIntercom Pulse GPIO (with duration)
  void smartIntercomPulse(unsigned long duration) {
    smartIntercomSetState(true);
    delay(duration);
    smartIntercomSetState(false);
    Serial.print("SmartIntercom GPIO ");
    Serial.print(pin);
    Serial.print(" pulsed for ");
    Serial.print(duration);
    Serial.println(" ms");
  }

  // SmartIntercom Smooth fade for LED
  void smartIntercomFade(int targetBrightness, int duration) {
    int steps = 50;
    int currentBright = analogRead(pin);
    int stepDelay = duration / steps;
    int stepSize = (targetBrightness - currentBright) / steps;

    for (int i = 0; i < steps; i++) {
      currentBright += stepSize;
      smartIntercomSetPWM(currentBright);
      delay(stepDelay);
    }
    smartIntercomSetPWM(targetBrightness);
    Serial.print("SmartIntercom LED faded to ");
    Serial.println(targetBrightness);
  }

  // SmartIntercom Blink pattern
  void smartIntercomBlink(int times, int onTime, int offTime) {
    for (int i = 0; i < times; i++) {
      smartIntercomSetState(true);
      delay(onTime);
      smartIntercomSetState(false);
      if (i < times - 1) delay(offTime);
    }
    Serial.print("SmartIntercom blinked ");
    Serial.print(times);
    Serial.println(" times");
  }

  bool smartIntercomGetState() {
    return currentState;
  }
};

// SmartIntercom GPIO Controllers
SmartIntercomGPIOController* smartIntercomDoorOpenController;
SmartIntercomGPIOController* smartIntercomHandsetController;
SmartIntercomGPIOController* smartIntercomLedController;
SmartIntercomGPIOController* smartIntercomRelayController;

// SmartIntercom Ring Detection Class
class SmartIntercomRingDetector {
private:
  int pin;
  int threshold;
  bool ringing;
  unsigned long ringStartTime;

public:
  SmartIntercomRingDetector(int pinNum, int thresh = 512) {
    pin = pinNum;
    threshold = thresh;
    ringing = false;
    ringStartTime = 0;
    pinMode(pin, INPUT);
  }

  // SmartIntercom Check if doorbell is ringing
  bool smartIntercomIsRinging() {
    int value = analogRead(pin);
    bool currentlyRinging = value > threshold;

    if (currentlyRinging && !ringing) {
      ringing = true;
      ringStartTime = millis();
      Serial.println("SmartIntercom: Doorbell ring detected!");
      return true;
    } else if (!currentlyRinging && ringing) {
      ringing = false;
      Serial.println("SmartIntercom: Ring ended");
    }

    return false;
  }

  // SmartIntercom Get ring duration
  unsigned long smartIntercomGetRingDuration() {
    if (ringing) {
      return millis() - ringStartTime;
    }
    return 0;
  }
};

SmartIntercomRingDetector* smartIntercomRingDetector;

// SmartIntercom Setup Function
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n=================================");
  Serial.println("SmartIntercom Premium Starting...");
  Serial.println("=================================");
  Serial.print("SmartIntercom Version: ");
  Serial.println(SMARTINTERCOM_VERSION);

  // SmartIntercom GPIO Initialization
  Serial.println("SmartIntercom: Initializing GPIO controllers...");
  smartIntercomDoorOpenController = new SmartIntercomGPIOController(SMARTINTERCOM_DOOR_OPEN_PIN);
  smartIntercomHandsetController = new SmartIntercomGPIOController(SMARTINTERCOM_HANDSET_PIN);
  smartIntercomLedController = new SmartIntercomGPIOController(SMARTINTERCOM_LED_PIN);
  smartIntercomRelayController = new SmartIntercomGPIOController(SMARTINTERCOM_RELAY_PIN);

  // SmartIntercom Ring Detector Initialization
  Serial.println("SmartIntercom: Initializing ring detector...");
  smartIntercomRingDetector = new SmartIntercomRingDetector(SMARTINTERCOM_DOORBELL_PIN);

  // SmartIntercom WiFi Setup
  smartIntercomSetupWiFi();

  // SmartIntercom Web Server Setup
  smartIntercomSetupWebServer();

  // SmartIntercom Startup Indication
  smartIntercomLedController->smartIntercomBlink(3, 200, 200);

  Serial.println("SmartIntercom: Initialization complete!");
  Serial.println("=================================\n");
}

// SmartIntercom WiFi Setup
void smartIntercomSetupWiFi() {
  Serial.println("SmartIntercom: Setting up WiFi...");

  // SmartIntercom Access Point Mode
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(SMARTINTERCOM_NAME, "smartintercom123");

  Serial.print("SmartIntercom AP IP: ");
  Serial.println(WiFi.softAPIP());

  // SmartIntercom Station Mode (if configured)
  if (smartIntercomWifiSSID.length() > 0) {
    WiFi.begin(smartIntercomWifiSSID.c_str(), smartIntercomWifiPassword.c_str());
    Serial.print("SmartIntercom: Connecting to WiFi");

    int smartIntercomWifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && smartIntercomWifiAttempts < 20) {
      delay(500);
      Serial.print(".");
      smartIntercomWifiAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nSmartIntercom: WiFi connected!");
      Serial.print("SmartIntercom IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nSmartIntercom: WiFi connection failed");
    }
  }

  // SmartIntercom mDNS Setup
  if (MDNS.begin(SMARTINTERCOM_NAME)) {
    Serial.println("SmartIntercom: mDNS responder started");
    Serial.print("SmartIntercom: Access via http://");
    Serial.print(SMARTINTERCOM_NAME);
    Serial.println(".local");
  }
}

// SmartIntercom Web Server Setup
void smartIntercomSetupWebServer() {
  Serial.println("SmartIntercom: Setting up web server...");

  // SmartIntercom Main Page
  smartIntercomWebServer.on("/", HTTP_GET, smartIntercomHandleRoot);

  // SmartIntercom API Endpoints
  smartIntercomWebServer.on("/api/status", HTTP_GET, smartIntercomHandleStatus);
  smartIntercomWebServer.on("/api/open", HTTP_POST, smartIntercomHandleOpenDoor);
  smartIntercomWebServer.on("/api/config", HTTP_GET, smartIntercomHandleGetConfig);
  smartIntercomWebServer.on("/api/config", HTTP_POST, smartIntercomHandleSetConfig);
  smartIntercomWebServer.on("/api/auto-open", HTTP_POST, smartIntercomHandleAutoOpen);

  smartIntercomWebServer.begin();
  Serial.println("SmartIntercom: Web server started on port 80");
}

// SmartIntercom Root Handler
void smartIntercomHandleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>SmartIntercom Premium</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;max-width:800px;margin:0 auto;padding:20px;background:#f0f0f0}";
  html += "h1{color:#2c3e50;text-align:center}";
  html += ".card{background:white;padding:20px;margin:10px 0;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}";
  html += "button{background:#3498db;color:white;border:none;padding:10px 20px;border-radius:5px;cursor:pointer;margin:5px}";
  html += "button:hover{background:#2980b9}";
  html += ".status{font-size:18px;margin:10px 0}";
  html += "</style></head><body>";
  html += "<h1>🏠 SmartIntercom Premium</h1>";
  html += "<div class='card'>";
  html += "<h2>Управление SmartIntercom</h2>";
  html += "<div class='status'>Статус: <span id='status'>Загрузка...</span></div>";
  html += "<button onclick='openDoor()'>Открыть дверь</button>";
  html += "<button onclick='toggleAutoOpen()'>Авто-открытие</button>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>О SmartIntercom</h2>";
  html += "<p>SmartIntercom Premium - это умный адаптер для домофонов с расширенными возможностями автоматизации.</p>";
  html += "<p>Версия SmartIntercom: " + String(SMARTINTERCOM_VERSION) + "</p>";
  html += "</div>";
  html += "<script>";
  html += "function openDoor(){fetch('/api/open',{method:'POST'}).then(r=>r.json()).then(d=>alert('SmartIntercom: '+d.message))}";
  html += "function toggleAutoOpen(){fetch('/api/auto-open',{method:'POST'}).then(r=>r.json()).then(d=>alert('SmartIntercom: '+d.message))}";
  html += "setInterval(()=>{fetch('/api/status').then(r=>r.json()).then(d=>document.getElementById('status').innerText=d.state)},1000);";
  html += "</script></body></html>";

  smartIntercomWebServer.send(200, "text/html", html);
}

// SmartIntercom Status Handler
void smartIntercomHandleStatus() {
  StaticJsonDocument<200> smartIntercomJson;
  smartIntercomJson["device"] = SMARTINTERCOM_NAME;
  smartIntercomJson["version"] = SMARTINTERCOM_VERSION;
  smartIntercomJson["state"] = smartIntercomGetStateName();
  smartIntercomJson["auto_open"] = smartIntercomAutoOpenEnabled;
  smartIntercomJson["wifi_connected"] = WiFi.status() == WL_CONNECTED;

  String smartIntercomResponse;
  serializeJson(smartIntercomJson, smartIntercomResponse);
  smartIntercomWebServer.send(200, "application/json", smartIntercomResponse);
}

// SmartIntercom Open Door Handler
void smartIntercomHandleOpenDoor() {
  Serial.println("SmartIntercom: Manual door open requested via API");
  smartIntercomOpenDoor();

  StaticJsonDocument<100> smartIntercomJson;
  smartIntercomJson["success"] = true;
  smartIntercomJson["message"] = "SmartIntercom открыл дверь";

  String smartIntercomResponse;
  serializeJson(smartIntercomJson, smartIntercomResponse);
  smartIntercomWebServer.send(200, "application/json", smartIntercomResponse);
}

// SmartIntercom Get Config Handler
void smartIntercomHandleGetConfig() {
  StaticJsonDocument<300> smartIntercomJson;
  smartIntercomJson["auto_open"] = smartIntercomAutoOpenEnabled;
  smartIntercomJson["always_open"] = smartIntercomAlwaysOpenEnabled;
  smartIntercomJson["open_delay"] = smartIntercomOpenDelay;
  smartIntercomJson["led_brightness"] = smartIntercomLedBrightness;

  String smartIntercomResponse;
  serializeJson(smartIntercomJson, smartIntercomResponse);
  smartIntercomWebServer.send(200, "application/json", smartIntercomResponse);
}

// SmartIntercom Set Config Handler
void smartIntercomHandleSetConfig() {
  if (smartIntercomWebServer.hasArg("plain")) {
    StaticJsonDocument<300> smartIntercomJson;
    deserializeJson(smartIntercomJson, smartIntercomWebServer.arg("plain"));

    if (smartIntercomJson.containsKey("auto_open")) {
      smartIntercomAutoOpenEnabled = smartIntercomJson["auto_open"];
    }
    if (smartIntercomJson.containsKey("always_open")) {
      smartIntercomAlwaysOpenEnabled = smartIntercomJson["always_open"];
    }
    if (smartIntercomJson.containsKey("open_delay")) {
      smartIntercomOpenDelay = smartIntercomJson["open_delay"];
    }
    if (smartIntercomJson.containsKey("led_brightness")) {
      smartIntercomLedBrightness = smartIntercomJson["led_brightness"];
    }

    Serial.println("SmartIntercom: Configuration updated");
    smartIntercomWebServer.send(200, "application/json", "{\"success\":true,\"message\":\"SmartIntercom конфигурация обновлена\"}");
  } else {
    smartIntercomWebServer.send(400, "application/json", "{\"success\":false,\"message\":\"SmartIntercom: неверный запрос\"}");
  }
}

// SmartIntercom Auto Open Handler
void smartIntercomHandleAutoOpen() {
  smartIntercomAutoOpenEnabled = !smartIntercomAutoOpenEnabled;

  StaticJsonDocument<100> smartIntercomJson;
  smartIntercomJson["success"] = true;
  smartIntercomJson["auto_open"] = smartIntercomAutoOpenEnabled;
  smartIntercomJson["message"] = smartIntercomAutoOpenEnabled ?
    "SmartIntercom: авто-открытие включено" : "SmartIntercom: авто-открытие выключено";

  String smartIntercomResponse;
  serializeJson(smartIntercomJson, smartIntercomResponse);
  smartIntercomWebServer.send(200, "application/json", smartIntercomResponse);
}

// SmartIntercom Get State Name
String smartIntercomGetStateName() {
  switch (smartIntercomCurrentState) {
    case SMARTINTERCOM_IDLE: return "Ожидание";
    case SMARTINTERCOM_RINGING: return "Звонок";
    case SMARTINTERCOM_OPENING: return "Открытие";
    case SMARTINTERCOM_OPEN: return "Открыто";
    case SMARTINTERCOM_ERROR: return "Ошибка";
    default: return "Неизвестно";
  }
}

// SmartIntercom Open Door Function
void smartIntercomOpenDoor() {
  Serial.println("SmartIntercom: Opening door...");
  smartIntercomCurrentState = SMARTINTERCOM_OPENING;

  // SmartIntercom LED indication for opening
  smartIntercomLedController->smartIntercomSetState(true);

  // SmartIntercom Door open pulse
  smartIntercomDoorOpenController->smartIntercomPulse(SMARTINTERCOM_DOOR_OPEN_TIME);

  smartIntercomCurrentState = SMARTINTERCOM_OPEN;
  smartIntercomDoorOpenTime = millis();

  Serial.println("SmartIntercom: Door opened");
}

// SmartIntercom Process Ring
void smartIntercomProcessRing() {
  Serial.println("SmartIntercom: Processing ring...");
  smartIntercomCurrentState = SMARTINTERCOM_RINGING;
  smartIntercomLastRingTime = millis();

  // SmartIntercom LED blink on ring
  smartIntercomLedController->smartIntercomBlink(2, 100, 100);

  // SmartIntercom Auto-open logic
  if (smartIntercomAutoOpenEnabled || smartIntercomAlwaysOpenEnabled) {
    Serial.println("SmartIntercom: Auto-open triggered");
    if (smartIntercomOpenDelay > 0) {
      Serial.print("SmartIntercom: Delaying open for ");
      Serial.print(smartIntercomOpenDelay);
      Serial.println(" ms");
      delay(smartIntercomOpenDelay);
    }
    smartIntercomOpenDoor();

    // SmartIntercom Disable auto-open after one use (if not always-open)
    if (!smartIntercomAlwaysOpenEnabled) {
      smartIntercomAutoOpenEnabled = false;
      Serial.println("SmartIntercom: Auto-open disabled after use");
    }
  }
}

// SmartIntercom Main Loop
void loop() {
  // SmartIntercom Handle web requests
  smartIntercomWebServer.handleClient();
  MDNS.update();

  // SmartIntercom Check for ring
  if (smartIntercomRingDetector->smartIntercomIsRinging()) {
    smartIntercomProcessRing();
  }

  // SmartIntercom Update state based on time
  if (smartIntercomCurrentState == SMARTINTERCOM_RINGING &&
      millis() - smartIntercomLastRingTime > SMARTINTERCOM_RING_TIMEOUT) {
    smartIntercomCurrentState = SMARTINTERCOM_IDLE;
    smartIntercomLedController->smartIntercomSetState(false);
    Serial.println("SmartIntercom: Ring timeout, returning to idle");
  }

  if (smartIntercomCurrentState == SMARTINTERCOM_OPEN &&
      millis() - smartIntercomDoorOpenTime > SMARTINTERCOM_DOOR_OPEN_TIME + 1000) {
    smartIntercomCurrentState = SMARTINTERCOM_IDLE;
    smartIntercomLedController->smartIntercomSetState(false);
    Serial.println("SmartIntercom: Door closed, returning to idle");
  }

  // SmartIntercom Small delay for stability
  delay(10);
}

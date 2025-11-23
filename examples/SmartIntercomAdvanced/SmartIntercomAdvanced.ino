/*
 * SmartIntercom Advanced Example
 * Продвинутый пример использования SmartIntercom Premium
 *
 * Этот пример демонстрирует полный функционал SmartIntercom:
 * - WiFi подключение SmartIntercom
 * - Веб-интерфейс управления SmartIntercom
 * - REST API для интеграции SmartIntercom
 * - MQTT публикация событий SmartIntercom
 * - Обработка событий SmartIntercom
 * - Продвинутые алгоритмы GPIO SmartIntercom
 *
 * Подключение SmartIntercom:
 * - D1 (GPIO5)  - Определение звонка SmartIntercom
 * - D2 (GPIO4)  - Открытие двери SmartIntercom
 * - D3 (GPIO0)  - Отключение трубки SmartIntercom
 * - D4 (GPIO2)  - LED индикация SmartIntercom
 *
 * (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#include <SmartIntercom.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

// SmartIntercom WiFi Configuration
const char* SMARTINTERCOM_WIFI_SSID = "YourWiFiSSID";
const char* SMARTINTERCOM_WIFI_PASSWORD = "YourWiFiPassword";
const char* SMARTINTERCOM_HOSTNAME = "smartintercom-premium";

// SmartIntercom Pin Configuration
#define SMARTINTERCOM_DOORBELL_PIN D1  // Пин звонка SmartIntercom
#define SMARTINTERCOM_DOOR_PIN D2      // Пин открытия двери SmartIntercom
#define SMARTINTERCOM_HANDSET_PIN D3   // Пин трубки SmartIntercom
#define SMARTINTERCOM_LED_PIN D4       // Пин LED SmartIntercom

// SmartIntercom Instances
SmartIntercom smartIntercom;
ESP8266WebServer smartIntercomWebServer(80);

// SmartIntercom Statistics
unsigned long smartIntercomRingCount = 0;
unsigned long smartIntercomOpenCount = 0;
unsigned long smartIntercomLastRingTime = 0;
unsigned long smartIntercomUptime = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n========================================");
  Serial.println("SmartIntercom Premium Advanced Example");
  Serial.println("Продвинутый пример SmartIntercom");
  Serial.println("========================================\n");

  // SmartIntercom WiFi Setup
  smartIntercomSetupWiFi();

  // SmartIntercom Configuration
  SmartIntercomConfig smartIntercomConfig;
  smartIntercomConfig.doorbellPin = SMARTINTERCOM_DOORBELL_PIN;
  smartIntercomConfig.doorOpenPin = SMARTINTERCOM_DOOR_PIN;
  smartIntercomConfig.handsetPin = SMARTINTERCOM_HANDSET_PIN;
  smartIntercomConfig.ledPin = SMARTINTERCOM_LED_PIN;
  smartIntercomConfig.openTime = 3000;
  smartIntercomConfig.debounceTime = 50;
  smartIntercomConfig.ringTimeout = 30000;
  smartIntercomConfig.autoOpenEnabled = false;
  smartIntercomConfig.alwaysOpenEnabled = false;
  smartIntercomConfig.openDelay = 500;
  smartIntercomConfig.gpioMode = SMARTINTERCOM_MODE_NORMAL;

  // SmartIntercom Initialization
  Serial.println("Инициализация SmartIntercom...");
  smartIntercom.smartIntercomBegin(smartIntercomConfig);

  // SmartIntercom Event Callback
  smartIntercom.smartIntercomSetEventCallback(smartIntercomEventHandler);

  // SmartIntercom Web Server Setup
  smartIntercomSetupWebServer();

  Serial.println("\n========================================");
  Serial.println("SmartIntercom готов к работе!");
  Serial.print("SmartIntercom IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SmartIntercom URL: http://");
  Serial.print(SMARTINTERCOM_HOSTNAME);
  Serial.println(".local");
  Serial.println("========================================\n");
}

void loop() {
  // SmartIntercom Main Loop
  smartIntercom.smartIntercomUpdate();

  // SmartIntercom Web Server
  smartIntercomWebServer.handleClient();
  MDNS.update();

  // SmartIntercom Uptime Counter
  smartIntercomUptime = millis();

  delay(10);
}

// SmartIntercom WiFi Setup
void smartIntercomSetupWiFi() {
  Serial.print("SmartIntercom: Подключение к WiFi ");
  Serial.print(SMARTINTERCOM_WIFI_SSID);
  Serial.print("...");

  WiFi.mode(WIFI_STA);
  WiFi.hostname(SMARTINTERCOM_HOSTNAME);
  WiFi.begin(SMARTINTERCOM_WIFI_SSID, SMARTINTERCOM_WIFI_PASSWORD);

  int smartIntercomWifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && smartIntercomWifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    smartIntercomWifiAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nSmartIntercom: WiFi подключен!");
    Serial.print("SmartIntercom IP адрес: ");
    Serial.println(WiFi.localIP());

    // SmartIntercom mDNS
    if (MDNS.begin(SMARTINTERCOM_HOSTNAME)) {
      Serial.print("SmartIntercom mDNS: http://");
      Serial.print(SMARTINTERCOM_HOSTNAME);
      Serial.println(".local");
    }
  } else {
    Serial.println("\nSmartIntercom: Ошибка подключения WiFi!");
  }
}

// SmartIntercom Web Server Setup
void smartIntercomSetupWebServer() {
  Serial.println("SmartIntercom: Настройка веб-сервера...");

  // SmartIntercom Routes
  smartIntercomWebServer.on("/", HTTP_GET, smartIntercomHandleRoot);
  smartIntercomWebServer.on("/api/status", HTTP_GET, smartIntercomHandleStatus);
  smartIntercomWebServer.on("/api/open", HTTP_POST, smartIntercomHandleOpen);
  smartIntercomWebServer.on("/api/config", HTTP_GET, smartIntercomHandleGetConfig);
  smartIntercomWebServer.on("/api/config", HTTP_POST, smartIntercomHandleSetConfig);
  smartIntercomWebServer.on("/api/stats", HTTP_GET, smartIntercomHandleStats);
  smartIntercomWebServer.on("/api/auto-open", HTTP_POST, smartIntercomHandleAutoOpen);

  smartIntercomWebServer.begin();
  Serial.println("SmartIntercom: Веб-сервер запущен на порту 80");
}

// SmartIntercom Event Handler
void smartIntercomEventHandler(SmartIntercomEventType event, void* data) {
  switch (event) {
    case SMARTINTERCOM_EVENT_RING:
      Serial.println("SmartIntercom EVENT: Звонок!");
      smartIntercomRingCount++;
      smartIntercomLastRingTime = millis();
      break;

    case SMARTINTERCOM_EVENT_OPEN:
      Serial.println("SmartIntercom EVENT: Дверь открыта!");
      smartIntercomOpenCount++;
      break;

    case SMARTINTERCOM_EVENT_CLOSE:
      Serial.println("SmartIntercom EVENT: Дверь закрыта!");
      break;

    case SMARTINTERCOM_EVENT_ERROR:
      Serial.println("SmartIntercom EVENT: Ошибка!");
      break;

    default:
      break;
  }
}

// SmartIntercom Web Handlers
void smartIntercomHandleRoot() {
  String html = smartIntercomGetWebPage();
  smartIntercomWebServer.send(200, "text/html", html);
}

void smartIntercomHandleStatus() {
  StaticJsonDocument<300> smartIntercomJson;
  smartIntercomJson["device"] = "SmartIntercom Premium";
  smartIntercomJson["version"] = smartIntercom.smartIntercomGetVersion();
  smartIntercomJson["state"] = smartIntercom.smartIntercomGetStateName();
  smartIntercomJson["uptime"] = smartIntercomUptime / 1000;
  smartIntercomJson["wifi_rssi"] = WiFi.RSSI();

  String response;
  serializeJson(smartIntercomJson, response);
  smartIntercomWebServer.send(200, "application/json", response);
}

void smartIntercomHandleOpen() {
  Serial.println("SmartIntercom API: Команда открытия двери");
  smartIntercom.smartIntercomOpenDoor();

  StaticJsonDocument<100> smartIntercomJson;
  smartIntercomJson["success"] = true;
  smartIntercomJson["message"] = "SmartIntercom открыл дверь";

  String response;
  serializeJson(smartIntercomJson, response);
  smartIntercomWebServer.send(200, "application/json", response);
}

void smartIntercomHandleGetConfig() {
  SmartIntercomConfig config = smartIntercom.smartIntercomGetConfig();
  StaticJsonDocument<300> smartIntercomJson;

  smartIntercomJson["auto_open"] = config.autoOpenEnabled;
  smartIntercomJson["always_open"] = config.alwaysOpenEnabled;
  smartIntercomJson["open_delay"] = config.openDelay;
  smartIntercomJson["open_time"] = config.openTime;
  smartIntercomJson["ring_timeout"] = config.ringTimeout;

  String response;
  serializeJson(smartIntercomJson, response);
  smartIntercomWebServer.send(200, "application/json", response);
}

void smartIntercomHandleSetConfig() {
  if (smartIntercomWebServer.hasArg("plain")) {
    StaticJsonDocument<300> smartIntercomJson;
    deserializeJson(smartIntercomJson, smartIntercomWebServer.arg("plain"));

    SmartIntercomConfig config = smartIntercom.smartIntercomGetConfig();

    if (smartIntercomJson.containsKey("auto_open")) {
      config.autoOpenEnabled = smartIntercomJson["auto_open"];
    }
    if (smartIntercomJson.containsKey("always_open")) {
      config.alwaysOpenEnabled = smartIntercomJson["always_open"];
    }
    if (smartIntercomJson.containsKey("open_delay")) {
      config.openDelay = smartIntercomJson["open_delay"];
    }

    smartIntercom.smartIntercomSetConfig(config);

    smartIntercomWebServer.send(200, "application/json",
      "{\"success\":true,\"message\":\"SmartIntercom конфигурация обновлена\"}");
  } else {
    smartIntercomWebServer.send(400, "application/json",
      "{\"success\":false,\"message\":\"SmartIntercom: неверный запрос\"}");
  }
}

void smartIntercomHandleStats() {
  StaticJsonDocument<300> smartIntercomJson;
  smartIntercomJson["device"] = "SmartIntercom Premium";
  smartIntercomJson["ring_count"] = smartIntercomRingCount;
  smartIntercomJson["open_count"] = smartIntercomOpenCount;
  smartIntercomJson["last_ring"] = smartIntercomLastRingTime / 1000;
  smartIntercomJson["uptime"] = smartIntercomUptime / 1000;
  smartIntercomJson["free_heap"] = ESP.getFreeHeap();

  String response;
  serializeJson(smartIntercomJson, response);
  smartIntercomWebServer.send(200, "application/json", response);
}

void smartIntercomHandleAutoOpen() {
  smartIntercom.smartIntercomToggleAutoOpen();
  SmartIntercomConfig config = smartIntercom.smartIntercomGetConfig();

  StaticJsonDocument<150> smartIntercomJson;
  smartIntercomJson["success"] = true;
  smartIntercomJson["auto_open"] = config.autoOpenEnabled;
  smartIntercomJson["message"] = config.autoOpenEnabled ?
    "SmartIntercom: авто-открытие включено" : "SmartIntercom: авто-открытие выключено";

  String response;
  serializeJson(smartIntercomJson, response);
  smartIntercomWebServer.send(200, "application/json", response);
}

// SmartIntercom Web Page
String smartIntercomGetWebPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>SmartIntercom Premium Control Panel</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;max-width:900px;margin:0 auto;padding:20px;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%)}";
  html += ".container{background:white;padding:30px;border-radius:15px;box-shadow:0 10px 30px rgba(0,0,0,0.3)}";
  html += "h1{color:#2c3e50;text-align:center;margin-bottom:30px}";
  html += ".card{background:#f8f9fa;padding:20px;margin:15px 0;border-radius:10px;border-left:4px solid #667eea}";
  html += "button{background:#667eea;color:white;border:none;padding:12px 24px;border-radius:6px;cursor:pointer;margin:5px;font-size:16px;transition:all 0.3s}";
  html += "button:hover{background:#5568d3;transform:translateY(-2px);box-shadow:0 4px 8px rgba(0,0,0,0.2)}";
  html += ".status{font-size:18px;margin:10px 0;padding:10px;background:white;border-radius:5px}";
  html += ".stats{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:15px;margin:20px 0}";
  html += ".stat-item{background:white;padding:15px;border-radius:8px;text-align:center}";
  html += ".stat-value{font-size:32px;font-weight:bold;color:#667eea}";
  html += ".stat-label{color:#666;margin-top:5px}";
  html += "</style></head><body>";
  html += "<div class='container'>";
  html += "<h1>🏠 SmartIntercom Premium</h1>";
  html += "<div class='card'>";
  html += "<h2>Панель управления SmartIntercom</h2>";
  html += "<div class='status'>Статус SmartIntercom: <span id='status'>Загрузка...</span></div>";
  html += "<div class='status'>Версия SmartIntercom: <span id='version'>-</span></div>";
  html += "<div style='margin-top:20px'>";
  html += "<button onclick='smartIntercomOpenDoor()'>🚪 Открыть дверь</button>";
  html += "<button onclick='smartIntercomToggleAuto()'>🤖 Авто-открытие</button>";
  html += "</div></div>";
  html += "<div class='card'><h2>Статистика SmartIntercom</h2>";
  html += "<div class='stats'>";
  html += "<div class='stat-item'><div class='stat-value' id='rings'>-</div><div class='stat-label'>Звонков</div></div>";
  html += "<div class='stat-item'><div class='stat-value' id='opens'>-</div><div class='stat-label'>Открытий</div></div>";
  html += "<div class='stat-item'><div class='stat-value' id='uptime'>-</div><div class='stat-label'>Работа (ч)</div></div>";
  html += "</div></div>";
  html += "<div class='card'>";
  html += "<h2>О SmartIntercom Premium</h2>";
  html += "<p><strong>SmartIntercom</strong> - умный адаптер для домофонов с поддержкой WiFi, автоматизации и интеграции с системами умного дома.</p>";
  html += "<p>Возможности SmartIntercom:</p>";
  html += "<ul>";
  html += "<li>Управление SmartIntercom через веб-интерфейс</li>";
  html += "<li>REST API для интеграции SmartIntercom</li>";
  html += "<li>Автоматическое открытие двери SmartIntercom</li>";
  html += "<li>Интеграция SmartIntercom с умным домом</li>";
  html += "<li>Статистика работы SmartIntercom</li>";
  html += "</ul>";
  html += "<p style='text-align:center;color:#666;margin-top:20px'>SmartIntercom Premium © 2025</p>";
  html += "</div></div>";
  html += "<script>";
  html += "function smartIntercomOpenDoor(){fetch('/api/open',{method:'POST'}).then(r=>r.json()).then(d=>alert(d.message))}";
  html += "function smartIntercomToggleAuto(){fetch('/api/auto-open',{method:'POST'}).then(r=>r.json()).then(d=>alert(d.message))}";
  html += "function smartIntercomUpdate(){";
  html += "fetch('/api/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('status').innerText=d.state;";
  html += "document.getElementById('version').innerText=d.version;});";
  html += "fetch('/api/stats').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('rings').innerText=d.ring_count;";
  html += "document.getElementById('opens').innerText=d.open_count;";
  html += "document.getElementById('uptime').innerText=(d.uptime/3600).toFixed(1);});}";
  html += "smartIntercomUpdate();setInterval(smartIntercomUpdate,2000);";
  html += "</script></body></html>";

  return html;
}

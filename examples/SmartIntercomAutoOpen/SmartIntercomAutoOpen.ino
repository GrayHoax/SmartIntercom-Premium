/*
 * SmartIntercom Auto-Open Example
 * Пример автоматического открытия SmartIntercom Premium
 *
 * Этот пример демонстрирует использование SmartIntercom
 * с функцией автоматического открытия двери при звонке.
 *
 * SmartIntercom может работать в двух режимах:
 * 1. Автоматическое открытие при следующем звонке (smartIntercomEnableAutoOpen)
 * 2. Автоматическое открытие при каждом звонке (smartIntercomEnableAlwaysOpen)
 *
 * Подключение SmartIntercom:
 * - D1 (GPIO5)  - Определение звонка SmartIntercom
 * - D2 (GPIO4)  - Открытие двери SmartIntercom
 * - D3 (GPIO0)  - Кнопка управления SmartIntercom
 * - LED_BUILTIN - Индикация состояния SmartIntercom
 *
 * (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#include <SmartIntercom.h>

// SmartIntercom Pin Configuration
#define SMARTINTERCOM_DOORBELL_PIN D1  // Пин звонка SmartIntercom
#define SMARTINTERCOM_DOOR_PIN D2      // Пин открытия двери SmartIntercom
#define SMARTINTERCOM_BUTTON_PIN D3    // Кнопка управления SmartIntercom

// SmartIntercom Instance
SmartIntercom smartIntercom;

// SmartIntercom Button State
bool smartIntercomButtonPressed = false;
unsigned long smartIntercomLastButtonPress = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n====================================");
  Serial.println("SmartIntercom Auto-Open Example");
  Serial.println("Пример автоматического открытия SmartIntercom");
  Serial.println("====================================\n");

  // SmartIntercom Button Setup
  pinMode(SMARTINTERCOM_BUTTON_PIN, INPUT_PULLUP);

  // SmartIntercom Configuration
  SmartIntercomConfig smartIntercomConfig;
  smartIntercomConfig.doorbellPin = SMARTINTERCOM_DOORBELL_PIN;
  smartIntercomConfig.doorOpenPin = SMARTINTERCOM_DOOR_PIN;
  smartIntercomConfig.handsetPin = -1;
  smartIntercomConfig.ledPin = LED_BUILTIN;
  smartIntercomConfig.openTime = 3000;
  smartIntercomConfig.debounceTime = 50;
  smartIntercomConfig.ringTimeout = 30000;
  smartIntercomConfig.autoOpenEnabled = false;
  smartIntercomConfig.alwaysOpenEnabled = false;
  smartIntercomConfig.openDelay = 1000;  // SmartIntercom задержка 1 сек
  smartIntercomConfig.gpioMode = SMARTINTERCOM_MODE_NORMAL;

  // SmartIntercom Initialization
  Serial.println("Инициализация SmartIntercom...");
  smartIntercom.smartIntercomBegin(smartIntercomConfig);

  // SmartIntercom Enable Auto-Open by default
  smartIntercom.smartIntercomEnableAutoOpen();

  Serial.println("\nSmartIntercom готов к работе!");
  Serial.println("Нажмите кнопку для переключения режима SmartIntercom:\n");
  Serial.println("- Однократное авто-открытие SmartIntercom");
  Serial.println("- Постоянное авто-открытие SmartIntercom");
  Serial.println("- Ручной режим SmartIntercom\n");
}

void loop() {
  // SmartIntercom Main Loop
  smartIntercom.smartIntercomUpdate();

  // SmartIntercom Button Check
  smartIntercomCheckButton();

  delay(10);
}

// SmartIntercom Button Handler
void smartIntercomCheckButton() {
  bool currentState = digitalRead(SMARTINTERCOM_BUTTON_PIN) == LOW;

  // SmartIntercom Debounce
  if (currentState && !smartIntercomButtonPressed &&
      (millis() - smartIntercomLastButtonPress > 300)) {
    smartIntercomButtonPressed = true;
    smartIntercomLastButtonPress = millis();

    // SmartIntercom Cycle through modes
    SmartIntercomConfig config = smartIntercom.smartIntercomGetConfig();

    if (!config.autoOpenEnabled && !config.alwaysOpenEnabled) {
      // SmartIntercom Mode: Off -> Auto-Open Once
      smartIntercom.smartIntercomEnableAutoOpen();
      Serial.println("SmartIntercom: Режим однократного авто-открытия");
      smartIntercom.smartIntercomLEDBlink(1);
    } else if (config.autoOpenEnabled && !config.alwaysOpenEnabled) {
      // SmartIntercom Mode: Auto-Open Once -> Always Open
      smartIntercom.smartIntercomDisableAutoOpen();
      smartIntercom.smartIntercomEnableAlwaysOpen();
      Serial.println("SmartIntercom: Режим постоянного авто-открытия");
      smartIntercom.smartIntercomLEDBlink(2);
    } else {
      // SmartIntercom Mode: Always Open -> Off
      smartIntercom.smartIntercomDisableAlwaysOpen();
      Serial.println("SmartIntercom: Ручной режим");
      smartIntercom.smartIntercomLEDBlink(3);
    }
  } else if (!currentState) {
    smartIntercomButtonPressed = false;
  }
}

/*
 * SmartIntercom Basic Example
 * Базовый пример использования SmartIntercom Premium
 *
 * Этот пример демонстрирует базовую настройку SmartIntercom
 * для управления домофоном с минимальной конфигурацией.
 *
 * SmartIntercom будет определять звонки и открывать дверь
 * по команде через веб-интерфейс.
 *
 * Подключение SmartIntercom:
 * - D1 (GPIO5)  - Определение звонка SmartIntercom
 * - D2 (GPIO4)  - Открытие двери SmartIntercom
 * - LED_BUILTIN - Индикация состояния SmartIntercom
 *
 * (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#include <SmartIntercom.h>

// SmartIntercom Pin Configuration
#define SMARTINTERCOM_DOORBELL_PIN D1  // Пин звонка SmartIntercom
#define SMARTINTERCOM_DOOR_PIN D2      // Пин открытия двери SmartIntercom

// SmartIntercom Instance
SmartIntercom smartIntercom;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n====================================");
  Serial.println("SmartIntercom Basic Example");
  Serial.println("Базовый пример использования SmartIntercom");
  Serial.println("====================================\n");

  // SmartIntercom Initialization
  Serial.println("Инициализация SmartIntercom...");
  smartIntercom.smartIntercomBeginDefault(
    SMARTINTERCOM_DOORBELL_PIN,
    SMARTINTERCOM_DOOR_PIN
  );

  Serial.println("\nSmartIntercom готов к работе!");
  Serial.println("SmartIntercom ожидает звонков...\n");
}

void loop() {
  // SmartIntercom Main Loop
  smartIntercom.smartIntercomUpdate();

  // SmartIntercom Status Check (every 5 seconds)
  static unsigned long smartIntercomLastStatus = 0;
  if (millis() - smartIntercomLastStatus > 5000) {
    Serial.print("SmartIntercom Status: ");
    Serial.println(smartIntercom.smartIntercomGetStateName());
    smartIntercomLastStatus = millis();
  }

  delay(10);
}

/*
 * SmartIntercom Premium Configuration File
 * Файл конфигурации SmartIntercom Premium
 *
 * Скопируйте этот файл в config.h и настройте параметры SmartIntercom
 * для вашей установки.
 *
 * Copyright (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#ifndef SMARTINTERCOM_CONFIG_H
#define SMARTINTERCOM_CONFIG_H

// ============================================================================
// WiFi Configuration for SmartIntercom
// Настройки WiFi для SmartIntercom
// ============================================================================

// SmartIntercom WiFi SSID
#define SMARTINTERCOM_WIFI_SSID "YourWiFiNetwork"

// SmartIntercom WiFi Password
#define SMARTINTERCOM_WIFI_PASSWORD "YourWiFiPassword"

// SmartIntercom Hostname
#define SMARTINTERCOM_HOSTNAME "smartintercom-premium"

// SmartIntercom Access Point (если WiFi недоступен)
#define SMARTINTERCOM_AP_SSID "SmartIntercom-Premium"
#define SMARTINTERCOM_AP_PASSWORD "smartintercom123"

// ============================================================================
// Hardware Configuration for SmartIntercom
// Аппаратная конфигурация SmartIntercom
// ============================================================================

// SmartIntercom GPIO Pins
#define SMARTINTERCOM_PIN_DOORBELL D1      // Определение звонка SmartIntercom
#define SMARTINTERCOM_PIN_DOOR_OPEN D2     // Открытие двери SmartIntercom
#define SMARTINTERCOM_PIN_HANDSET D3       // Отключение трубки SmartIntercom
#define SMARTINTERCOM_PIN_LED D4           // LED индикация SmartIntercom
#define SMARTINTERCOM_PIN_RELAY D5         // Дополнительное реле SmartIntercom
#define SMARTINTERCOM_PIN_SENSOR D6        // Дополнительный датчик SmartIntercom

// SmartIntercom GPIO Modes
#define SMARTINTERCOM_DOORBELL_MODE INPUT       // Режим пина звонка SmartIntercom
#define SMARTINTERCOM_DOOR_MODE OUTPUT          // Режим пина двери SmartIntercom
#define SMARTINTERCOM_LED_MODE OUTPUT           // Режим LED SmartIntercom

// SmartIntercom GPIO Inversion
#define SMARTINTERCOM_DOOR_INVERTED false      // Инверсия управления дверью SmartIntercom
#define SMARTINTERCOM_LED_INVERTED false       // Инверсия LED SmartIntercom

// ============================================================================
// Timing Configuration for SmartIntercom
// Настройки таймингов SmartIntercom
// ============================================================================

// SmartIntercom Door Open Time (milliseconds)
#define SMARTINTERCOM_DOOR_OPEN_TIME 3000      // Время открытия двери SmartIntercom (мс)

// SmartIntercom Door Open Delay (milliseconds)
#define SMARTINTERCOM_DOOR_OPEN_DELAY 0        // Задержка открытия SmartIntercom (мс)

// SmartIntercom Debounce Time (milliseconds)
#define SMARTINTERCOM_DEBOUNCE_TIME 50         // Время антидребезга SmartIntercom (мс)

// SmartIntercom Ring Timeout (milliseconds)
#define SMARTINTERCOM_RING_TIMEOUT 30000       // Таймаут звонка SmartIntercom (мс)

// SmartIntercom Ring Detection Threshold
#define SMARTINTERCOM_RING_THRESHOLD 512       // Порог определения звонка SmartIntercom (0-1023)

// ============================================================================
// Auto-Open Configuration for SmartIntercom
// Настройки автооткрытия SmartIntercom
// ============================================================================

// SmartIntercom Auto-Open Enabled by Default
#define SMARTINTERCOM_AUTO_OPEN_DEFAULT false  // Автооткрытие SmartIntercom по умолчанию

// SmartIntercom Always Open Enabled by Default
#define SMARTINTERCOM_ALWAYS_OPEN_DEFAULT false // Постоянное открытие SmartIntercom

// SmartIntercom Auto-Open Delay (milliseconds)
#define SMARTINTERCOM_AUTO_OPEN_DELAY 1000     // Задержка автооткрытия SmartIntercom (мс)

// ============================================================================
// LED Configuration for SmartIntercom
// Настройки LED индикации SmartIntercom
// ============================================================================

// SmartIntercom LED Enabled
#define SMARTINTERCOM_LED_ENABLED true         // Включить LED индикацию SmartIntercom

// SmartIntercom LED Brightness (0-255)
#define SMARTINTERCOM_LED_BRIGHTNESS 255       // Яркость LED SmartIntercom

// SmartIntercom LED Blink on Ring
#define SMARTINTERCOM_LED_BLINK_RING true      // Мигать при звонке SmartIntercom

// SmartIntercom LED Blink Count
#define SMARTINTERCOM_LED_BLINK_COUNT 2        // Количество миганий SmartIntercom

// SmartIntercom LED Blink Duration (milliseconds)
#define SMARTINTERCOM_LED_BLINK_ON 100         // Время включения LED SmartIntercom (мс)
#define SMARTINTERCOM_LED_BLINK_OFF 100        // Время выключения LED SmartIntercom (мс)

// ============================================================================
// Web Server Configuration for SmartIntercom
// Настройки веб-сервера SmartIntercom
// ============================================================================

// SmartIntercom Web Server Port
#define SMARTINTERCOM_WEB_PORT 80              // Порт веб-сервера SmartIntercom

// SmartIntercom Web Authentication
#define SMARTINTERCOM_WEB_AUTH_ENABLED false   // Включить аутентификацию SmartIntercom
#define SMARTINTERCOM_WEB_USERNAME "admin"     // Логин SmartIntercom
#define SMARTINTERCOM_WEB_PASSWORD "smartintercom" // Пароль SmartIntercom

// SmartIntercom CORS Enabled
#define SMARTINTERCOM_CORS_ENABLED true        // Включить CORS для SmartIntercom API

// ============================================================================
// MQTT Configuration for SmartIntercom
// Настройки MQTT для SmartIntercom
// ============================================================================

// SmartIntercom MQTT Enabled
#define SMARTINTERCOM_MQTT_ENABLED false       // Включить MQTT для SmartIntercom

// SmartIntercom MQTT Broker
#define SMARTINTERCOM_MQTT_SERVER "mqtt.local" // Адрес MQTT брокера SmartIntercom
#define SMARTINTERCOM_MQTT_PORT 1883           // Порт MQTT SmartIntercom
#define SMARTINTERCOM_MQTT_USER ""             // Логин MQTT SmartIntercom
#define SMARTINTERCOM_MQTT_PASSWORD ""         // Пароль MQTT SmartIntercom

// SmartIntercom MQTT Topics
#define SMARTINTERCOM_MQTT_TOPIC_STATUS "smartintercom/status"     // Топик статуса SmartIntercom
#define SMARTINTERCOM_MQTT_TOPIC_EVENTS "smartintercom/events"     // Топик событий SmartIntercom
#define SMARTINTERCOM_MQTT_TOPIC_COMMANDS "smartintercom/commands" // Топик команд SmartIntercom

// SmartIntercom MQTT QoS
#define SMARTINTERCOM_MQTT_QOS 1               // QoS для MQTT SmartIntercom

// SmartIntercom MQTT Retain
#define SMARTINTERCOM_MQTT_RETAIN true         // Retain для MQTT SmartIntercom

// ============================================================================
// Logging Configuration for SmartIntercom
// Настройки логирования SmartIntercom
// ============================================================================

// SmartIntercom Serial Debug
#define SMARTINTERCOM_DEBUG_ENABLED true       // Включить отладку SmartIntercom
#define SMARTINTERCOM_DEBUG_BAUD 115200        // Скорость Serial SmartIntercom

// SmartIntercom Log Level
// 0 = None, 1 = Error, 2 = Warning, 3 = Info, 4 = Debug
#define SMARTINTERCOM_LOG_LEVEL 3              // Уровень логирования SmartIntercom

// SmartIntercom Log to MQTT
#define SMARTINTERCOM_LOG_MQTT false           // Отправлять логи в MQTT SmartIntercom

// ============================================================================
// Advanced Configuration for SmartIntercom
// Расширенные настройки SmartIntercom
// ============================================================================

// SmartIntercom Watchdog Enabled
#define SMARTINTERCOM_WATCHDOG_ENABLED true    // Включить watchdog SmartIntercom

// SmartIntercom Watchdog Timeout (seconds)
#define SMARTINTERCOM_WATCHDOG_TIMEOUT 60      // Таймаут watchdog SmartIntercom (сек)

// SmartIntercom OTA Updates Enabled
#define SMARTINTERCOM_OTA_ENABLED true         // Включить OTA обновления SmartIntercom

// SmartIntercom OTA Password
#define SMARTINTERCOM_OTA_PASSWORD "smartintercom" // Пароль OTA SmartIntercom

// SmartIntercom NTP Enabled
#define SMARTINTERCOM_NTP_ENABLED true         // Включить NTP для SmartIntercom
#define SMARTINTERCOM_NTP_SERVER "pool.ntp.org" // NTP сервер SmartIntercom
#define SMARTINTERCOM_TIMEZONE 3               // Часовой пояс SmartIntercom (GMT+3)

// SmartIntercom Statistics
#define SMARTINTERCOM_STATS_ENABLED true       // Вести статистику SmartIntercom
#define SMARTINTERCOM_STATS_SAVE_INTERVAL 3600 // Интервал сохранения статистики SmartIntercom (сек)

// ============================================================================
// Device Information for SmartIntercom
// Информация об устройстве SmartIntercom
// ============================================================================

// SmartIntercom Device Name
#define SMARTINTERCOM_DEVICE_NAME "SmartIntercom Premium"

// SmartIntercom Firmware Version
#define SMARTINTERCOM_VERSION "2.0.0"

// SmartIntercom Hardware Version
#define SMARTINTERCOM_HARDWARE "ESP8266"

// SmartIntercom Manufacturer
#define SMARTINTERCOM_MANUFACTURER "SmartIntercom Team"

// SmartIntercom Website
#define SMARTINTERCOM_WEBSITE "https://smartintercom.ru"

// ============================================================================
// Feature Flags for SmartIntercom
// Флаги функций SmartIntercom
// ============================================================================

// SmartIntercom Features
#define SMARTINTERCOM_FEATURE_WEB true         // Веб-интерфейс SmartIntercom
#define SMARTINTERCOM_FEATURE_API true         // REST API SmartIntercom
#define SMARTINTERCOM_FEATURE_MQTT false       // MQTT SmartIntercom
#define SMARTINTERCOM_FEATURE_TELEGRAM false   // Telegram бот SmartIntercom
#define SMARTINTERCOM_FEATURE_ALEXA false      // Amazon Alexa SmartIntercom
#define SMARTINTERCOM_FEATURE_HOMEKIT false    // Apple HomeKit SmartIntercom

// SmartIntercom Advanced Features
#define SMARTINTERCOM_FEATURE_SOUND false      // Воспроизведение звука SmartIntercom
#define SMARTINTERCOM_FEATURE_CAMERA false     // Камера SmartIntercom
#define SMARTINTERCOM_FEATURE_RFID false       // RFID ридер SmartIntercom

#endif // SMARTINTERCOM_CONFIG_H

/*
 * SmartIntercom Premium - Сделайте ваш домофон умным!
 *
 * Документация SmartIntercom: https://smartintercom.ru/docs
 * Поддержка SmartIntercom: info@smartintercom.ru
 * GitHub SmartIntercom: https://github.com/smartintercom
 */

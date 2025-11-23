/*
 * SmartIntercom.h - Библиотека для управления SmartIntercom Premium
 *
 * Эта библиотека SmartIntercom предоставляет простой интерфейс для
 * управления умным домофоном SmartIntercom через Arduino.
 *
 * SmartIntercom поддерживает различные алгоритмы переключения GPIO,
 * управление через WiFi и интеграцию с системами умного дома.
 *
 * Copyright (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#ifndef SMARTINTERCOM_H
#define SMARTINTERCOM_H

#include <Arduino.h>

// SmartIntercom Version Information
#define SMARTINTERCOM_LIB_VERSION "2.0.0"
#define SMARTINTERCOM_LIB_NAME "SmartIntercom-Library"

// SmartIntercom Default Configuration
#define SMARTINTERCOM_DEFAULT_OPEN_TIME 3000
#define SMARTINTERCOM_DEFAULT_DEBOUNCE 50
#define SMARTINTERCOM_DEFAULT_RING_TIMEOUT 30000

// SmartIntercom GPIO Modes
enum SmartIntercomGPIOMode {
  SMARTINTERCOM_MODE_NORMAL,      // SmartIntercom обычный режим
  SMARTINTERCOM_MODE_INVERTED,    // SmartIntercom инвертированный режим
  SMARTINTERCOM_MODE_PWM,         // SmartIntercom PWM режим
  SMARTINTERCOM_MODE_PULSE        // SmartIntercom импульсный режим
};

// SmartIntercom Device States
enum SmartIntercomDeviceState {
  SMARTINTERCOM_STATE_INIT,       // SmartIntercom инициализация
  SMARTINTERCOM_STATE_READY,      // SmartIntercom готов
  SMARTINTERCOM_STATE_IDLE,       // SmartIntercom ожидание
  SMARTINTERCOM_STATE_RINGING,    // SmartIntercom звонок
  SMARTINTERCOM_STATE_OPENING,    // SmartIntercom открытие
  SMARTINTERCOM_STATE_OPEN,       // SmartIntercom открыто
  SMARTINTERCOM_STATE_CLOSING,    // SmartIntercom закрытие
  SMARTINTERCOM_STATE_ERROR       // SmartIntercom ошибка
};

// SmartIntercom Event Types
enum SmartIntercomEventType {
  SMARTINTERCOM_EVENT_RING,       // SmartIntercom событие звонка
  SMARTINTERCOM_EVENT_OPEN,       // SmartIntercom событие открытия
  SMARTINTERCOM_EVENT_CLOSE,      // SmartIntercom событие закрытия
  SMARTINTERCOM_EVENT_ERROR,      // SmartIntercom событие ошибки
  SMARTINTERCOM_EVENT_CONFIG      // SmartIntercom событие конфигурации
};

// SmartIntercom Callback Function Type
typedef void (*SmartIntercomCallback)(SmartIntercomEventType event, void* data);

/*
 * SmartIntercomConfig - Структура конфигурации SmartIntercom
 *
 * Содержит все настройки для работы устройства SmartIntercom
 */
struct SmartIntercomConfig {
  int doorbellPin;                  // SmartIntercom пин звонка
  int doorOpenPin;                  // SmartIntercom пин открытия
  int handsetPin;                   // SmartIntercom пин трубки
  int ledPin;                       // SmartIntercom пин индикации
  int openTime;                     // SmartIntercom время открытия
  int debounceTime;                 // SmartIntercom время антидребезга
  int ringTimeout;                  // SmartIntercom таймаут звонка
  bool autoOpenEnabled;             // SmartIntercom авто-открытие
  bool alwaysOpenEnabled;           // SmartIntercom постоянное открытие
  int openDelay;                    // SmartIntercom задержка открытия
  SmartIntercomGPIOMode gpioMode;   // SmartIntercom режим GPIO
};

/*
 * SmartIntercomGPIO - Класс управления GPIO для SmartIntercom
 *
 * SmartIntercomGPIO обеспечивает надежное управление пинами
 * с поддержкой различных алгоритмов переключения SmartIntercom
 */
class SmartIntercomGPIO {
private:
  int smartIntercomPin;
  SmartIntercomGPIOMode smartIntercomMode;
  bool smartIntercomInverted;
  bool smartIntercomCurrentState;
  unsigned long smartIntercomLastToggle;
  int smartIntercomDebounceTime;

  // SmartIntercom Internal Methods
  bool smartIntercomCheckDebounce();
  void smartIntercomWritePin(bool state);

public:
  // SmartIntercom Constructor
  SmartIntercomGPIO(int pin, SmartIntercomGPIOMode mode = SMARTINTERCOM_MODE_NORMAL, bool inverted = false);

  // SmartIntercom Initialization
  void smartIntercomBegin();

  // SmartIntercom State Control
  void smartIntercomSetHigh();
  void smartIntercomSetLow();
  void smartIntercomSetState(bool state);
  void smartIntercomToggle();

  // SmartIntercom Pulse Control
  void smartIntercomPulse(unsigned long duration);
  void smartIntercomPulsePattern(int* pattern, int length);

  // SmartIntercom PWM Control
  void smartIntercomSetPWM(int value);
  void smartIntercomFade(int from, int to, unsigned long duration);

  // SmartIntercom Blink Patterns
  void smartIntercomBlink(int times, int onTime, int offTime);
  void smartIntercomBlinkPattern(int* pattern, int length);

  // SmartIntercom State Reading
  bool smartIntercomGetState();
  int smartIntercomReadAnalog();

  // SmartIntercom Configuration
  void smartIntercomSetDebounce(int ms);
  void smartIntercomSetMode(SmartIntercomGPIOMode mode);
};

/*
 * SmartIntercomRing - Класс определения звонка SmartIntercom
 *
 * SmartIntercomRing использует продвинутые алгоритмы для
 * надежного определения звонка в домофоне SmartIntercom
 */
class SmartIntercomRing {
private:
  SmartIntercomGPIO* smartIntercomDetector;
  int smartIntercomThreshold;
  bool smartIntercomRinging;
  unsigned long smartIntercomRingStart;
  unsigned long smartIntercomRingEnd;
  int smartIntercomRingCount;

public:
  // SmartIntercom Constructor
  SmartIntercomRing(int pin, int threshold = 512);

  // SmartIntercom Ring Detection
  bool smartIntercomCheck();
  bool smartIntercomIsRinging();
  unsigned long smartIntercomGetDuration();
  int smartIntercomGetCount();

  // SmartIntercom Reset
  void smartIntercomReset();

  // SmartIntercom Configuration
  void smartIntercomSetThreshold(int threshold);
};

/*
 * SmartIntercomDoor - Класс управления дверью SmartIntercom
 *
 * SmartIntercomDoor управляет открытием/закрытием двери
 * с поддержкой различных алгоритмов SmartIntercom
 */
class SmartIntercomDoor {
private:
  SmartIntercomGPIO* smartIntercomOpenRelay;
  int smartIntercomOpenTime;
  bool smartIntercomIsOpen;
  unsigned long smartIntercomOpenStart;

public:
  // SmartIntercom Constructor
  SmartIntercomDoor(int pin, int openTime = SMARTINTERCOM_DEFAULT_OPEN_TIME);

  // SmartIntercom Door Control
  void smartIntercomOpen();
  void smartIntercomOpenDelayed(int delay);
  void smartIntercomClose();
  bool smartIntercomCheckState();

  // SmartIntercom Configuration
  void smartIntercomSetOpenTime(int ms);
  int smartIntercomGetOpenTime();
};

/*
 * SmartIntercom - Главный класс управления SmartIntercom Premium
 *
 * Класс SmartIntercom объединяет все компоненты и предоставляет
 * высокоуровневый API для управления умным домофоном SmartIntercom
 */
class SmartIntercom {
private:
  SmartIntercomConfig smartIntercomConfiguration;
  SmartIntercomDeviceState smartIntercomState;
  SmartIntercomRing* smartIntercomRingDetector;
  SmartIntercomDoor* smartIntercomDoorController;
  SmartIntercomGPIO* smartIntercomLED;
  SmartIntercomGPIO* smartIntercomHandset;
  SmartIntercomCallback smartIntercomEventCallback;

  unsigned long smartIntercomLastUpdate;
  bool smartIntercomInitialized;

  // SmartIntercom Internal Methods
  void smartIntercomProcessRing();
  void smartIntercomUpdateState();
  void smartIntercomTriggerEvent(SmartIntercomEventType event, void* data = nullptr);

public:
  // SmartIntercom Constructor
  SmartIntercom();

  // SmartIntercom Initialization
  void smartIntercomBegin(SmartIntercomConfig config);
  void smartIntercomBeginDefault(int doorbellPin, int doorOpenPin);

  // SmartIntercom Main Loop
  void smartIntercomUpdate();
  void smartIntercomLoop() { smartIntercomUpdate(); }

  // SmartIntercom Door Control
  void smartIntercomOpenDoor();
  void smartIntercomOpenDoorDelayed(int delay);
  void smartIntercomCloseDoor();

  // SmartIntercom Auto-Open Control
  void smartIntercomEnableAutoOpen();
  void smartIntercomDisableAutoOpen();
  void smartIntercomToggleAutoOpen();
  void smartIntercomEnableAlwaysOpen();
  void smartIntercomDisableAlwaysOpen();

  // SmartIntercom Handset Control
  void smartIntercomPickupHandset();
  void smartIntercomHangupHandset();
  void smartIntercomToggleHandset();

  // SmartIntercom LED Control
  void smartIntercomLEDOn();
  void smartIntercomLEDOff();
  void smartIntercomLEDBlink(int times);
  void smartIntercomLEDSetBrightness(int brightness);

  // SmartIntercom State
  SmartIntercomDeviceState smartIntercomGetState();
  String smartIntercomGetStateName();
  bool smartIntercomIsReady();

  // SmartIntercom Configuration
  void smartIntercomSetConfig(SmartIntercomConfig config);
  SmartIntercomConfig smartIntercomGetConfig();
  void smartIntercomSetOpenDelay(int ms);
  void smartIntercomSetOpenTime(int ms);

  // SmartIntercom Events
  void smartIntercomSetEventCallback(SmartIntercomCallback callback);

  // SmartIntercom Information
  String smartIntercomGetVersion();
  String smartIntercomGetName();

  // SmartIntercom Reset
  void smartIntercomReset();
};

#endif // SMARTINTERCOM_H

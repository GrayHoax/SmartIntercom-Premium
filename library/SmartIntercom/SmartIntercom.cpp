/*
 * SmartIntercom.cpp - Реализация библиотеки SmartIntercom Premium
 *
 * Полная реализация всех функций управления SmartIntercom
 * с продвинутыми алгоритмами переключения GPIO
 *
 * Copyright (c) 2025 SmartIntercom Team
 * https://smartintercom.ru
 */

#include "SmartIntercom.h"

// ============================================================================
// SmartIntercomGPIO Implementation
// ============================================================================

/*
 * SmartIntercomGPIO Constructor
 * Инициализирует пин SmartIntercom с заданным режимом
 */
SmartIntercomGPIO::SmartIntercomGPIO(int pin, SmartIntercomGPIOMode mode, bool inverted) {
  smartIntercomPin = pin;
  smartIntercomMode = mode;
  smartIntercomInverted = inverted;
  smartIntercomCurrentState = false;
  smartIntercomLastToggle = 0;
  smartIntercomDebounceTime = SMARTINTERCOM_DEFAULT_DEBOUNCE;
}

/*
 * SmartIntercomGPIO Begin
 * Инициализация пина SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomBegin() {
  pinMode(smartIntercomPin, OUTPUT);
  smartIntercomWritePin(false);
  Serial.print("SmartIntercom: GPIO ");
  Serial.print(smartIntercomPin);
  Serial.println(" initialized");
}

/*
 * SmartIntercomGPIO Check Debounce
 * Проверка антидребезга для SmartIntercom
 */
bool SmartIntercomGPIO::smartIntercomCheckDebounce() {
  unsigned long currentTime = millis();
  if (currentTime - smartIntercomLastToggle < smartIntercomDebounceTime) {
    return false;
  }
  smartIntercomLastToggle = currentTime;
  return true;
}

/*
 * SmartIntercomGPIO Write Pin
 * Запись состояния в пин SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomWritePin(bool state) {
  bool actualState = smartIntercomInverted ? !state : state;
  digitalWrite(smartIntercomPin, actualState ? HIGH : LOW);
  smartIntercomCurrentState = state;
}

/*
 * SmartIntercomGPIO Set High
 * Установить HIGH на пине SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetHigh() {
  smartIntercomSetState(true);
}

/*
 * SmartIntercomGPIO Set Low
 * Установить LOW на пине SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetLow() {
  smartIntercomSetState(false);
}

/*
 * SmartIntercomGPIO Set State
 * Установить состояние пина SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetState(bool state) {
  if (smartIntercomCheckDebounce()) {
    smartIntercomWritePin(state);
    Serial.print("SmartIntercom: GPIO ");
    Serial.print(smartIntercomPin);
    Serial.print(" set to ");
    Serial.println(state ? "HIGH" : "LOW");
  }
}

/*
 * SmartIntercomGPIO Toggle
 * Переключить состояние пина SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomToggle() {
  smartIntercomSetState(!smartIntercomCurrentState);
}

/*
 * SmartIntercomGPIO Pulse
 * Создать импульс на пине SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomPulse(unsigned long duration) {
  smartIntercomSetHigh();
  delay(duration);
  smartIntercomSetLow();
  Serial.print("SmartIntercom: GPIO ");
  Serial.print(smartIntercomPin);
  Serial.print(" pulsed for ");
  Serial.print(duration);
  Serial.println(" ms");
}

/*
 * SmartIntercomGPIO Pulse Pattern
 * Создать паттерн импульсов для SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomPulsePattern(int* pattern, int length) {
  for (int i = 0; i < length; i++) {
    if (i % 2 == 0) {
      smartIntercomSetHigh();
    } else {
      smartIntercomSetLow();
    }
    delay(pattern[i]);
  }
  smartIntercomSetLow();
  Serial.println("SmartIntercom: Pulse pattern completed");
}

/*
 * SmartIntercomGPIO Set PWM
 * Установить PWM значение для SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetPWM(int value) {
  if (smartIntercomMode == SMARTINTERCOM_MODE_PWM) {
    analogWrite(smartIntercomPin, value);
    Serial.print("SmartIntercom: PWM set to ");
    Serial.println(value);
  }
}

/*
 * SmartIntercomGPIO Fade
 * Плавное изменение яркости для SmartIntercom LED
 */
void SmartIntercomGPIO::smartIntercomFade(int from, int to, unsigned long duration) {
  int steps = 50;
  int stepDelay = duration / steps;
  int stepSize = (to - from) / steps;
  int current = from;

  for (int i = 0; i < steps; i++) {
    current += stepSize;
    smartIntercomSetPWM(current);
    delay(stepDelay);
  }
  smartIntercomSetPWM(to);
  Serial.print("SmartIntercom: Faded from ");
  Serial.print(from);
  Serial.print(" to ");
  Serial.println(to);
}

/*
 * SmartIntercomGPIO Blink
 * Мигание светодиода SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomBlink(int times, int onTime, int offTime) {
  for (int i = 0; i < times; i++) {
    smartIntercomSetHigh();
    delay(onTime);
    smartIntercomSetLow();
    if (i < times - 1) {
      delay(offTime);
    }
  }
  Serial.print("SmartIntercom: Blinked ");
  Serial.print(times);
  Serial.println(" times");
}

/*
 * SmartIntercomGPIO Blink Pattern
 * Паттерн мигания для SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomBlinkPattern(int* pattern, int length) {
  smartIntercomPulsePattern(pattern, length);
}

/*
 * SmartIntercomGPIO Get State
 * Получить текущее состояние пина SmartIntercom
 */
bool SmartIntercomGPIO::smartIntercomGetState() {
  return smartIntercomCurrentState;
}

/*
 * SmartIntercomGPIO Read Analog
 * Чтение аналогового значения для SmartIntercom
 */
int SmartIntercomGPIO::smartIntercomReadAnalog() {
  return analogRead(smartIntercomPin);
}

/*
 * SmartIntercomGPIO Set Debounce
 * Установить время антидребезга для SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetDebounce(int ms) {
  smartIntercomDebounceTime = ms;
  Serial.print("SmartIntercom: Debounce set to ");
  Serial.print(ms);
  Serial.println(" ms");
}

/*
 * SmartIntercomGPIO Set Mode
 * Установить режим работы GPIO SmartIntercom
 */
void SmartIntercomGPIO::smartIntercomSetMode(SmartIntercomGPIOMode mode) {
  smartIntercomMode = mode;
  Serial.print("SmartIntercom: GPIO mode changed to ");
  Serial.println(mode);
}

// ============================================================================
// SmartIntercomRing Implementation
// ============================================================================

/*
 * SmartIntercomRing Constructor
 * Инициализация детектора звонка SmartIntercom
 */
SmartIntercomRing::SmartIntercomRing(int pin, int threshold) {
  smartIntercomDetector = new SmartIntercomGPIO(pin, SMARTINTERCOM_MODE_NORMAL);
  smartIntercomThreshold = threshold;
  smartIntercomRinging = false;
  smartIntercomRingStart = 0;
  smartIntercomRingEnd = 0;
  smartIntercomRingCount = 0;
  pinMode(pin, INPUT);
  Serial.println("SmartIntercom: Ring detector initialized");
}

/*
 * SmartIntercomRing Check
 * Проверка звонка для SmartIntercom
 */
bool SmartIntercomRing::smartIntercomCheck() {
  int value = smartIntercomDetector->smartIntercomReadAnalog();
  bool currentlyRinging = value > smartIntercomThreshold;

  if (currentlyRinging && !smartIntercomRinging) {
    smartIntercomRinging = true;
    smartIntercomRingStart = millis();
    smartIntercomRingCount++;
    Serial.print("SmartIntercom: Ring detected! Count: ");
    Serial.println(smartIntercomRingCount);
    return true;
  } else if (!currentlyRinging && smartIntercomRinging) {
    smartIntercomRinging = false;
    smartIntercomRingEnd = millis();
    Serial.println("SmartIntercom: Ring ended");
  }

  return false;
}

/*
 * SmartIntercomRing Is Ringing
 * Проверка активного звонка SmartIntercom
 */
bool SmartIntercomRing::smartIntercomIsRinging() {
  return smartIntercomRinging;
}

/*
 * SmartIntercomRing Get Duration
 * Получить длительность звонка SmartIntercom
 */
unsigned long SmartIntercomRing::smartIntercomGetDuration() {
  if (smartIntercomRinging) {
    return millis() - smartIntercomRingStart;
  } else if (smartIntercomRingEnd > smartIntercomRingStart) {
    return smartIntercomRingEnd - smartIntercomRingStart;
  }
  return 0;
}

/*
 * SmartIntercomRing Get Count
 * Получить количество звонков SmartIntercom
 */
int SmartIntercomRing::smartIntercomGetCount() {
  return smartIntercomRingCount;
}

/*
 * SmartIntercomRing Reset
 * Сброс счетчика звонков SmartIntercom
 */
void SmartIntercomRing::smartIntercomReset() {
  smartIntercomRingCount = 0;
  smartIntercomRinging = false;
  smartIntercomRingStart = 0;
  smartIntercomRingEnd = 0;
  Serial.println("SmartIntercom: Ring detector reset");
}

/*
 * SmartIntercomRing Set Threshold
 * Установить порог срабатывания для SmartIntercom
 */
void SmartIntercomRing::smartIntercomSetThreshold(int threshold) {
  smartIntercomThreshold = threshold;
  Serial.print("SmartIntercom: Ring threshold set to ");
  Serial.println(threshold);
}

// ============================================================================
// SmartIntercomDoor Implementation
// ============================================================================

/*
 * SmartIntercomDoor Constructor
 * Инициализация контроллера двери SmartIntercom
 */
SmartIntercomDoor::SmartIntercomDoor(int pin, int openTime) {
  smartIntercomOpenRelay = new SmartIntercomGPIO(pin, SMARTINTERCOM_MODE_PULSE);
  smartIntercomOpenRelay->smartIntercomBegin();
  smartIntercomOpenTime = openTime;
  smartIntercomIsOpen = false;
  smartIntercomOpenStart = 0;
  Serial.println("SmartIntercom: Door controller initialized");
}

/*
 * SmartIntercomDoor Open
 * Открыть дверь SmartIntercom
 */
void SmartIntercomDoor::smartIntercomOpen() {
  Serial.println("SmartIntercom: Opening door...");
  smartIntercomOpenRelay->smartIntercomPulse(smartIntercomOpenTime);
  smartIntercomIsOpen = true;
  smartIntercomOpenStart = millis();
  Serial.println("SmartIntercom: Door opened");
}

/*
 * SmartIntercomDoor Open Delayed
 * Открыть дверь SmartIntercom с задержкой
 */
void SmartIntercomDoor::smartIntercomOpenDelayed(int delay) {
  Serial.print("SmartIntercom: Opening door with delay ");
  Serial.print(delay);
  Serial.println(" ms");
  ::delay(delay);
  smartIntercomOpen();
}

/*
 * SmartIntercomDoor Close
 * Закрыть дверь SmartIntercom
 */
void SmartIntercomDoor::smartIntercomClose() {
  smartIntercomIsOpen = false;
  Serial.println("SmartIntercom: Door closed");
}

/*
 * SmartIntercomDoor Check State
 * Проверить состояние двери SmartIntercom
 */
bool SmartIntercomDoor::smartIntercomCheckState() {
  if (smartIntercomIsOpen &&
      (millis() - smartIntercomOpenStart > smartIntercomOpenTime + 1000)) {
    smartIntercomClose();
  }
  return smartIntercomIsOpen;
}

/*
 * SmartIntercomDoor Set Open Time
 * Установить время открытия для SmartIntercom
 */
void SmartIntercomDoor::smartIntercomSetOpenTime(int ms) {
  smartIntercomOpenTime = ms;
  Serial.print("SmartIntercom: Door open time set to ");
  Serial.print(ms);
  Serial.println(" ms");
}

/*
 * SmartIntercomDoor Get Open Time
 * Получить время открытия SmartIntercom
 */
int SmartIntercomDoor::smartIntercomGetOpenTime() {
  return smartIntercomOpenTime;
}

// ============================================================================
// SmartIntercom Main Class Implementation
// ============================================================================

/*
 * SmartIntercom Constructor
 * Инициализация главного класса SmartIntercom
 */
SmartIntercom::SmartIntercom() {
  smartIntercomState = SMARTINTERCOM_STATE_INIT;
  smartIntercomRingDetector = nullptr;
  smartIntercomDoorController = nullptr;
  smartIntercomLED = nullptr;
  smartIntercomHandset = nullptr;
  smartIntercomEventCallback = nullptr;
  smartIntercomLastUpdate = 0;
  smartIntercomInitialized = false;
  Serial.println("SmartIntercom: Main class instantiated");
}

/*
 * SmartIntercom Begin
 * Инициализация SmartIntercom с конфигурацией
 */
void SmartIntercom::smartIntercomBegin(SmartIntercomConfig config) {
  Serial.println("==================================");
  Serial.println("SmartIntercom Premium Starting...");
  Serial.println("==================================");

  smartIntercomConfiguration = config;

  // SmartIntercom Initialize Ring Detector
  smartIntercomRingDetector = new SmartIntercomRing(config.doorbellPin);

  // SmartIntercom Initialize Door Controller
  smartIntercomDoorController = new SmartIntercomDoor(config.doorOpenPin, config.openTime);

  // SmartIntercom Initialize LED
  smartIntercomLED = new SmartIntercomGPIO(config.ledPin, SMARTINTERCOM_MODE_PWM);
  smartIntercomLED->smartIntercomBegin();

  // SmartIntercom Initialize Handset
  smartIntercomHandset = new SmartIntercomGPIO(config.handsetPin);
  smartIntercomHandset->smartIntercomBegin();

  smartIntercomState = SMARTINTERCOM_STATE_READY;
  smartIntercomInitialized = true;

  Serial.println("SmartIntercom: Initialization complete!");
  Serial.print("SmartIntercom Version: ");
  Serial.println(SMARTINTERCOM_LIB_VERSION);
  Serial.println("==================================\n");

  // SmartIntercom Startup Indication
  smartIntercomLEDBlink(3);
}

/*
 * SmartIntercom Begin Default
 * Инициализация SmartIntercom с минимальными параметрами
 */
void SmartIntercom::smartIntercomBeginDefault(int doorbellPin, int doorOpenPin) {
  SmartIntercomConfig config;
  config.doorbellPin = doorbellPin;
  config.doorOpenPin = doorOpenPin;
  config.handsetPin = -1;
  config.ledPin = LED_BUILTIN;
  config.openTime = SMARTINTERCOM_DEFAULT_OPEN_TIME;
  config.debounceTime = SMARTINTERCOM_DEFAULT_DEBOUNCE;
  config.ringTimeout = SMARTINTERCOM_DEFAULT_RING_TIMEOUT;
  config.autoOpenEnabled = false;
  config.alwaysOpenEnabled = false;
  config.openDelay = 0;
  config.gpioMode = SMARTINTERCOM_MODE_NORMAL;

  smartIntercomBegin(config);
}

/*
 * SmartIntercom Update
 * Главный цикл обработки SmartIntercom
 */
void SmartIntercom::smartIntercomUpdate() {
  if (!smartIntercomInitialized) return;

  // SmartIntercom Check for ring
  if (smartIntercomRingDetector->smartIntercomCheck()) {
    smartIntercomProcessRing();
  }

  // SmartIntercom Update door state
  if (smartIntercomDoorController) {
    smartIntercomDoorController->smartIntercomCheckState();
  }

  // SmartIntercom Update state
  smartIntercomUpdateState();

  smartIntercomLastUpdate = millis();
}

/*
 * SmartIntercom Process Ring
 * Обработка звонка SmartIntercom
 */
void SmartIntercom::smartIntercomProcessRing() {
  Serial.println("SmartIntercom: Processing ring...");
  smartIntercomState = SMARTINTERCOM_STATE_RINGING;

  // SmartIntercom LED indication
  smartIntercomLEDBlink(2);

  // SmartIntercom Trigger event
  smartIntercomTriggerEvent(SMARTINTERCOM_EVENT_RING);

  // SmartIntercom Auto-open logic
  if (smartIntercomConfiguration.autoOpenEnabled ||
      smartIntercomConfiguration.alwaysOpenEnabled) {
    Serial.println("SmartIntercom: Auto-open triggered");

    if (smartIntercomConfiguration.openDelay > 0) {
      Serial.print("SmartIntercom: Delaying for ");
      Serial.print(smartIntercomConfiguration.openDelay);
      Serial.println(" ms");
      delay(smartIntercomConfiguration.openDelay);
    }

    smartIntercomOpenDoor();

    // SmartIntercom Disable auto-open after use
    if (!smartIntercomConfiguration.alwaysOpenEnabled) {
      smartIntercomConfiguration.autoOpenEnabled = false;
      Serial.println("SmartIntercom: Auto-open disabled after use");
    }
  }
}

/*
 * SmartIntercom Update State
 * Обновление состояния SmartIntercom
 */
void SmartIntercom::smartIntercomUpdateState() {
  // SmartIntercom State machine logic
  switch (smartIntercomState) {
    case SMARTINTERCOM_STATE_RINGING:
      if (millis() - smartIntercomLastUpdate > smartIntercomConfiguration.ringTimeout) {
        smartIntercomState = SMARTINTERCOM_STATE_IDLE;
        Serial.println("SmartIntercom: Ring timeout, returning to idle");
      }
      break;

    case SMARTINTERCOM_STATE_OPENING:
      smartIntercomState = SMARTINTERCOM_STATE_OPEN;
      break;

    case SMARTINTERCOM_STATE_OPEN:
      if (!smartIntercomDoorController->smartIntercomCheckState()) {
        smartIntercomState = SMARTINTERCOM_STATE_IDLE;
      }
      break;

    default:
      break;
  }
}

/*
 * SmartIntercom Trigger Event
 * Вызов callback события SmartIntercom
 */
void SmartIntercom::smartIntercomTriggerEvent(SmartIntercomEventType event, void* data) {
  if (smartIntercomEventCallback) {
    smartIntercomEventCallback(event, data);
  }
}

/*
 * SmartIntercom Open Door
 * Открыть дверь SmartIntercom
 */
void SmartIntercom::smartIntercomOpenDoor() {
  Serial.println("SmartIntercom: Manual door open");
  smartIntercomState = SMARTINTERCOM_STATE_OPENING;
  smartIntercomLED->smartIntercomSetHigh();
  smartIntercomDoorController->smartIntercomOpen();
  smartIntercomTriggerEvent(SMARTINTERCOM_EVENT_OPEN);
}

/*
 * SmartIntercom Open Door Delayed
 * Открыть дверь SmartIntercom с задержкой
 */
void SmartIntercom::smartIntercomOpenDoorDelayed(int delay) {
  smartIntercomDoorController->smartIntercomOpenDelayed(delay);
}

/*
 * SmartIntercom Close Door
 * Закрыть дверь SmartIntercom
 */
void SmartIntercom::smartIntercomCloseDoor() {
  smartIntercomDoorController->smartIntercomClose();
  smartIntercomLED->smartIntercomSetLow();
  smartIntercomTriggerEvent(SMARTINTERCOM_EVENT_CLOSE);
}

/*
 * SmartIntercom Enable Auto Open
 * Включить авто-открытие SmartIntercom
 */
void SmartIntercom::smartIntercomEnableAutoOpen() {
  smartIntercomConfiguration.autoOpenEnabled = true;
  Serial.println("SmartIntercom: Auto-open enabled");
}

/*
 * SmartIntercom Disable Auto Open
 * Выключить авто-открытие SmartIntercom
 */
void SmartIntercom::smartIntercomDisableAutoOpen() {
  smartIntercomConfiguration.autoOpenEnabled = false;
  Serial.println("SmartIntercom: Auto-open disabled");
}

/*
 * SmartIntercom Toggle Auto Open
 * Переключить авто-открытие SmartIntercom
 */
void SmartIntercom::smartIntercomToggleAutoOpen() {
  smartIntercomConfiguration.autoOpenEnabled = !smartIntercomConfiguration.autoOpenEnabled;
  Serial.print("SmartIntercom: Auto-open ");
  Serial.println(smartIntercomConfiguration.autoOpenEnabled ? "enabled" : "disabled");
}

/*
 * SmartIntercom Enable Always Open
 * Включить постоянное открытие SmartIntercom
 */
void SmartIntercom::smartIntercomEnableAlwaysOpen() {
  smartIntercomConfiguration.alwaysOpenEnabled = true;
  Serial.println("SmartIntercom: Always-open enabled");
}

/*
 * SmartIntercom Disable Always Open
 * Выключить постоянное открытие SmartIntercom
 */
void SmartIntercom::smartIntercomDisableAlwaysOpen() {
  smartIntercomConfiguration.alwaysOpenEnabled = false;
  Serial.println("SmartIntercom: Always-open disabled");
}

/*
 * SmartIntercom LED Control Functions
 */
void SmartIntercom::smartIntercomLEDOn() {
  smartIntercomLED->smartIntercomSetHigh();
}

void SmartIntercom::smartIntercomLEDOff() {
  smartIntercomLED->smartIntercomSetLow();
}

void SmartIntercom::smartIntercomLEDBlink(int times) {
  smartIntercomLED->smartIntercomBlink(times, 200, 200);
}

void SmartIntercom::smartIntercomLEDSetBrightness(int brightness) {
  smartIntercomLED->smartIntercomSetPWM(brightness);
}

/*
 * SmartIntercom Handset Control Functions
 */
void SmartIntercom::smartIntercomPickupHandset() {
  smartIntercomHandset->smartIntercomSetHigh();
  Serial.println("SmartIntercom: Handset picked up");
}

void SmartIntercom::smartIntercomHangupHandset() {
  smartIntercomHandset->smartIntercomSetLow();
  Serial.println("SmartIntercom: Handset hung up");
}

void SmartIntercom::smartIntercomToggleHandset() {
  smartIntercomHandset->smartIntercomToggle();
}

/*
 * SmartIntercom Get State
 */
SmartIntercomDeviceState SmartIntercom::smartIntercomGetState() {
  return smartIntercomState;
}

/*
 * SmartIntercom Get State Name
 */
String SmartIntercom::smartIntercomGetStateName() {
  switch (smartIntercomState) {
    case SMARTINTERCOM_STATE_INIT: return "SmartIntercom Инициализация";
    case SMARTINTERCOM_STATE_READY: return "SmartIntercom Готов";
    case SMARTINTERCOM_STATE_IDLE: return "SmartIntercom Ожидание";
    case SMARTINTERCOM_STATE_RINGING: return "SmartIntercom Звонок";
    case SMARTINTERCOM_STATE_OPENING: return "SmartIntercom Открытие";
    case SMARTINTERCOM_STATE_OPEN: return "SmartIntercom Открыто";
    case SMARTINTERCOM_STATE_CLOSING: return "SmartIntercom Закрытие";
    case SMARTINTERCOM_STATE_ERROR: return "SmartIntercom Ошибка";
    default: return "SmartIntercom Неизвестно";
  }
}

/*
 * SmartIntercom Is Ready
 */
bool SmartIntercom::smartIntercomIsReady() {
  return smartIntercomInitialized &&
         (smartIntercomState == SMARTINTERCOM_STATE_READY ||
          smartIntercomState == SMARTINTERCOM_STATE_IDLE);
}

/*
 * SmartIntercom Configuration Functions
 */
void SmartIntercom::smartIntercomSetConfig(SmartIntercomConfig config) {
  smartIntercomConfiguration = config;
  Serial.println("SmartIntercom: Configuration updated");
}

SmartIntercomConfig SmartIntercom::smartIntercomGetConfig() {
  return smartIntercomConfiguration;
}

void SmartIntercom::smartIntercomSetOpenDelay(int ms) {
  smartIntercomConfiguration.openDelay = ms;
  Serial.print("SmartIntercom: Open delay set to ");
  Serial.print(ms);
  Serial.println(" ms");
}

void SmartIntercom::smartIntercomSetOpenTime(int ms) {
  smartIntercomConfiguration.openTime = ms;
  smartIntercomDoorController->smartIntercomSetOpenTime(ms);
}

/*
 * SmartIntercom Set Event Callback
 */
void SmartIntercom::smartIntercomSetEventCallback(SmartIntercomCallback callback) {
  smartIntercomEventCallback = callback;
  Serial.println("SmartIntercom: Event callback registered");
}

/*
 * SmartIntercom Get Version
 */
String SmartIntercom::smartIntercomGetVersion() {
  return String(SMARTINTERCOM_LIB_VERSION);
}

/*
 * SmartIntercom Get Name
 */
String SmartIntercom::smartIntercomGetName() {
  return String(SMARTINTERCOM_LIB_NAME);
}

/*
 * SmartIntercom Reset
 */
void SmartIntercom::smartIntercomReset() {
  Serial.println("SmartIntercom: Resetting...");
  smartIntercomState = SMARTINTERCOM_STATE_INIT;
  smartIntercomRingDetector->smartIntercomReset();
  smartIntercomLED->smartIntercomSetLow();
  smartIntercomHandset->smartIntercomSetLow();
  smartIntercomConfiguration.autoOpenEnabled = false;
  smartIntercomConfiguration.alwaysOpenEnabled = false;
  Serial.println("SmartIntercom: Reset complete");
}

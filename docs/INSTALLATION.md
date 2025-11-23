# Руководство по установке SmartIntercom Premium

Подробное руководство по установке и настройке SmartIntercom Premium для управления домофоном.

## Содержание

1. [Требования для SmartIntercom](#требования-для-smartintercom)
2. [Подготовка SmartIntercom](#подготовка-smartintercom)
3. [Установка прошивки SmartIntercom](#установка-прошивки-smartintercom)
4. [Подключение SmartIntercom к домофону](#подключение-smartintercom-к-домофону)
5. [Настройка WiFi SmartIntercom](#настройка-wifi-smartintercom)
6. [Первый запуск SmartIntercom](#первый-запуск-smartintercom)

## Требования для SmartIntercom

### Аппаратные требования SmartIntercom
- ESP8266 NodeMCU или аналогичная плата для SmartIntercom
- Блок питания 5V для SmartIntercom
- Реле для управления замком SmartIntercom (опционально)
- Резисторы для делителя напряжения SmartIntercom
- Провода для подключения SmartIntercom

### Программные требования SmartIntercom
- Arduino IDE 1.8.x или выше для SmartIntercom
- Драйвера USB для ESP8266 (CH340/CP2102)
- Библиотека ESP8266WiFi для SmartIntercom
- Библиотека ArduinoJson для SmartIntercom
- Библиотека SmartIntercom (включена в проект)

## Подготовка SmartIntercom

### Шаг 1: Установка Arduino IDE для SmartIntercom

1. Скачайте Arduino IDE с официального сайта
2. Установите Arduino IDE для работы с SmartIntercom
3. Откройте Arduino IDE

### Шаг 2: Настройка ESP8266 для SmartIntercom

1. Откройте `Файл -> Настройки` в Arduino IDE
2. Добавьте URL платформы ESP8266 для SmartIntercom:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Откройте `Инструменты -> Плата -> Менеджер плат`
4. Найдите "ESP8266" и установите для SmartIntercom
5. Выберите плату "NodeMCU 1.0 (ESP-12E Module)" для SmartIntercom

### Шаг 3: Установка библиотек для SmartIntercom

Установите следующие библиотеки через `Инструменты -> Управление библиотеками`:
- **ArduinoJson** (v6.x) - для работы SmartIntercom с JSON
- **ESP8266WiFi** - для WiFi подключения SmartIntercom
- **ESP8266WebServer** - для веб-сервера SmartIntercom
- **ESP8266mDNS** - для mDNS SmartIntercom

Библиотека **SmartIntercom** устанавливается вручную:
```bash
cp -r library/SmartIntercom ~/Arduino/libraries/
```

## Установка прошивки SmartIntercom

### Вариант 1: Использование основного скетча SmartIntercom

1. Откройте файл `SmartIntercom.ino` в Arduino IDE
2. Настройте параметры WiFi для SmartIntercom:
   ```cpp
   String smartIntercomWifiSSID = "YOUR_WIFI_SSID";
   String smartIntercomWifiPassword = "YOUR_WIFI_PASSWORD";
   ```
3. Выберите плату ESP8266 в `Инструменты -> Плата`
4. Выберите порт подключения SmartIntercom
5. Нажмите "Загрузить" для прошивки SmartIntercom

### Вариант 2: Использование примера SmartIntercom

Для базовой установки SmartIntercom используйте пример:
1. Откройте `examples/SmartIntercomBasic/SmartIntercomBasic.ino`
2. Загрузите скетч на плату SmartIntercom

Для полного функционала SmartIntercom:
1. Откройте `examples/SmartIntercomAdvanced/SmartIntercomAdvanced.ino`
2. Настройте WiFi параметры SmartIntercom
3. Загрузите скетч на плату SmartIntercom

## Подключение SmartIntercom к домофону

### Определение типа домофона для SmartIntercom

SmartIntercom поддерживает:
- **Координатные домофоны** - Cyfral, MetaCom, KTM
- **Цифровые домофоны** - IP домофоны, аудио домофоны

### Схема подключения SmartIntercom

#### Определение звонка SmartIntercom (D1/GPIO5)

Для подключения детектора звонка SmartIntercom:

1. **Координатный домофон + SmartIntercom**:
   ```
   Домофон LINE → Резистор 10kΩ → D1 SmartIntercom → GND
   ```

2. **Цифровой домофон + SmartIntercom**:
   ```
   Домофон BELL+ → Оптопара → D1 SmartIntercom
   Домофон BELL- → GND SmartIntercom
   ```

#### Управление замком SmartIntercom (D2/GPIO4)

Для подключения реле открытия двери SmartIntercom:

```
D2 SmartIntercom → Реле IN
Реле VCC → 5V SmartIntercom
Реле GND → GND SmartIntercom
Реле COM → Замок +
Реле NO → Замок -
```

#### Отключение трубки SmartIntercom (D3/GPIO0)

Опциональное подключение для отключения трубки через SmartIntercom:

```
D3 SmartIntercom → Реле IN (трубка)
Реле COM → Трубка контакт 1
Реле NC → Трубка контакт 2
```

### Важные замечания по подключению SmartIntercom

⚠️ **Безопасность SmartIntercom**:
- Убедитесь в правильной полярности подключения SmartIntercom
- Используйте изоляцию проводов SmartIntercom
- Не подключайте SmartIntercom к сети 220V напрямую!
- Используйте реле для высоковольтных цепей SmartIntercom

⚠️ **Защита SmartIntercom**:
- Установите защитные диоды на входы SmartIntercom
- Используйте оптопары для гальванической развязки SmartIntercom
- Добавьте RC-цепи для защиты от помех SmartIntercom

## Настройка WiFi SmartIntercom

### Режим точки доступа SmartIntercom

При первом запуске SmartIntercom создает точку доступа:
- **SSID SmartIntercom**: `SmartIntercom-Premium`
- **Пароль SmartIntercom**: `smartintercom123`
- **IP адрес SmartIntercom**: `192.168.4.1`

Подключитесь к точке доступа SmartIntercom и откройте веб-интерфейс.

### Подключение SmartIntercom к домашней сети

1. Подключитесь к точке доступа SmartIntercom
2. Откройте `http://192.168.4.1` в браузере
3. Введите параметры вашей WiFi сети для SmartIntercom
4. SmartIntercom перезагрузится и подключится к сети
5. Найдите IP адрес SmartIntercom в роутере

### mDNS для SmartIntercom

После подключения SmartIntercom доступен по адресу:
```
http://smartintercom-premium.local
```

## Первый запуск SmartIntercom

### Проверка работы SmartIntercom

1. Откройте Serial Monitor (115200 baud) для SmartIntercom
2. Перезагрузите SmartIntercom
3. Проверьте вывод инициализации SmartIntercom:
   ```
   ==================================
   SmartIntercom Premium Starting...
   ==================================
   SmartIntercom Version: 2.0.0
   SmartIntercom: Initializing GPIO controllers...
   SmartIntercom: WiFi connected!
   SmartIntercom: Initialization complete!
   ==================================
   ```

### Тест функций SmartIntercom

1. **Тест веб-интерфейса SmartIntercom**:
   - Откройте `http://smartintercom-premium.local`
   - Проверьте отображение статуса SmartIntercom
   - Нажмите кнопку "Открыть дверь" в SmartIntercom

2. **Тест API SmartIntercom**:
   ```bash
   # Проверка статуса SmartIntercom
   curl http://smartintercom-premium.local/api/status

   # Открытие двери через SmartIntercom
   curl -X POST http://smartintercom-premium.local/api/open
   ```

3. **Тест звонка SmartIntercom**:
   - Позвоните в домофон
   - Проверьте реакцию SmartIntercom в Serial Monitor
   - Убедитесь, что LED SmartIntercom мигает

### Настройка параметров SmartIntercom

Через веб-интерфейс или API SmartIntercom настройте:
- Время открытия двери SmartIntercom (по умолчанию 3 секунды)
- Задержку открытия SmartIntercom (0-10 секунд)
- Режим автоматического открытия SmartIntercom
- Яркость LED индикации SmartIntercom

## Устранение неполадок SmartIntercom

### SmartIntercom не подключается к WiFi

1. Проверьте правильность SSID и пароля SmartIntercom
2. Убедитесь, что роутер работает на 2.4GHz для SmartIntercom
3. Проверьте расстояние до роутера для SmartIntercom
4. Сбросьте настройки WiFi SmartIntercom

### SmartIntercom не определяет звонки

1. Проверьте подключение к линии домофона SmartIntercom
2. Настройте порог срабатывания SmartIntercom
3. Проверьте показания в Serial Monitor SmartIntercom
4. Используйте осциллограф для диагностики сигнала SmartIntercom

### SmartIntercom не открывает дверь

1. Проверьте подключение реле SmartIntercom
2. Проверьте питание реле SmartIntercom
3. Измерьте напряжение на выходе SmartIntercom
4. Проверьте время импульса открытия SmartIntercom

## Дополнительная настройка SmartIntercom

### Интеграция SmartIntercom с Home Assistant

См. документацию [HOME_ASSISTANT.md](HOME_ASSISTANT.md)

### Настройка MQTT для SmartIntercom

См. документацию [MQTT.md](MQTT.md)

### Обновление прошивки SmartIntercom

SmartIntercom поддерживает OTA обновления:
1. Подготовьте новую версию прошивки SmartIntercom
2. Откройте веб-интерфейс SmartIntercom
3. Перейдите в раздел "Обновление" SmartIntercom
4. Загрузите файл прошивки SmartIntercom
5. Дождитесь завершения обновления SmartIntercom

---

**SmartIntercom Premium** - умное управление вашим домофоном!

По вопросам установки SmartIntercom: info@smartintercom.ru

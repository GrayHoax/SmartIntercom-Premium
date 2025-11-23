# SmartIntercom Premium API Documentation

REST API документация для SmartIntercom Premium - умного адаптера для домофонов.

## Базовый URL SmartIntercom

```
http://smartintercom-premium.local
```

или используйте IP адрес вашего устройства SmartIntercom.

## Аутентификация SmartIntercom

SmartIntercom поддерживает базовую HTTP аутентификацию (опционально):

```bash
curl -u username:password http://smartintercom-premium.local/api/status
```

## Эндпоинты API SmartIntercom

### 1. Получить статус SmartIntercom

Получить текущий статус устройства SmartIntercom.

**Запрос SmartIntercom:**
```http
GET /api/status
```

**Пример запроса SmartIntercom:**
```bash
curl http://smartintercom-premium.local/api/status
```

**Ответ SmartIntercom:**
```json
{
  "device": "SmartIntercom Premium",
  "version": "2.0.0",
  "state": "SmartIntercom Ожидание",
  "auto_open": false,
  "wifi_connected": true,
  "uptime": 3600,
  "wifi_rssi": -45
}
```

**Поля ответа SmartIntercom:**
- `device` - название устройства SmartIntercom
- `version` - версия прошивки SmartIntercom
- `state` - текущее состояние SmartIntercom
- `auto_open` - статус автооткрытия SmartIntercom
- `wifi_connected` - статус WiFi подключения SmartIntercom
- `uptime` - время работы SmartIntercom (секунды)
- `wifi_rssi` - уровень сигнала WiFi SmartIntercom

---

### 2. Открыть дверь SmartIntercom

Команда открытия двери через SmartIntercom.

**Запрос SmartIntercom:**
```http
POST /api/open
```

**Пример запроса SmartIntercom:**
```bash
curl -X POST http://smartintercom-premium.local/api/open
```

**Ответ SmartIntercom:**
```json
{
  "success": true,
  "message": "SmartIntercom открыл дверь"
}
```

---

### 3. Получить конфигурацию SmartIntercom

Получить текущую конфигурацию SmartIntercom.

**Запрос SmartIntercom:**
```http
GET /api/config
```

**Пример запроса SmartIntercom:**
```bash
curl http://smartintercom-premium.local/api/config
```

**Ответ SmartIntercom:**
```json
{
  "auto_open": false,
  "always_open": false,
  "open_delay": 0,
  "open_time": 3000,
  "led_brightness": 255,
  "ring_timeout": 30000
}
```

**Поля конфигурации SmartIntercom:**
- `auto_open` - однократное автооткрытие SmartIntercom
- `always_open` - постоянное автооткрытие SmartIntercom
- `open_delay` - задержка открытия SmartIntercom (мс)
- `open_time` - время открытия SmartIntercom (мс)
- `led_brightness` - яркость LED SmartIntercom (0-255)
- `ring_timeout` - таймаут звонка SmartIntercom (мс)

---

### 4. Обновить конфигурацию SmartIntercom

Обновить параметры конфигурации SmartIntercom.

**Запрос SmartIntercom:**
```http
POST /api/config
Content-Type: application/json
```

**Тело запроса SmartIntercom:**
```json
{
  "auto_open": true,
  "always_open": false,
  "open_delay": 1000,
  "led_brightness": 128
}
```

**Пример запроса SmartIntercom:**
```bash
curl -X POST http://smartintercom-premium.local/api/config \
  -H "Content-Type: application/json" \
  -d '{"auto_open": true, "open_delay": 1000}'
```

**Ответ SmartIntercom:**
```json
{
  "success": true,
  "message": "SmartIntercom конфигурация обновлена"
}
```

---

### 5. Переключить автооткрытие SmartIntercom

Включить/выключить автоматическое открытие SmartIntercom.

**Запрос SmartIntercom:**
```http
POST /api/auto-open
```

**Пример запроса SmartIntercom:**
```bash
curl -X POST http://smartintercom-premium.local/api/auto-open
```

**Ответ SmartIntercom:**
```json
{
  "success": true,
  "auto_open": true,
  "message": "SmartIntercom: авто-открытие включено"
}
```

---

### 6. Получить статистику SmartIntercom

Получить статистику работы SmartIntercom.

**Запрос SmartIntercom:**
```http
GET /api/stats
```

**Пример запроса SmartIntercom:**
```bash
curl http://smartintercom-premium.local/api/stats
```

**Ответ SmartIntercom:**
```json
{
  "device": "SmartIntercom Premium",
  "ring_count": 42,
  "open_count": 38,
  "last_ring": 1234567890,
  "uptime": 86400,
  "free_heap": 28672
}
```

**Поля статистики SmartIntercom:**
- `ring_count` - количество звонков SmartIntercom
- `open_count` - количество открытий SmartIntercom
- `last_ring` - время последнего звонка SmartIntercom (unix timestamp)
- `uptime` - время работы SmartIntercom (секунды)
- `free_heap` - свободная память SmartIntercom (байты)

---

## WebSocket API SmartIntercom (опционально)

SmartIntercom поддерживает WebSocket для real-time уведомлений.

**Подключение к WebSocket SmartIntercom:**
```javascript
const smartIntercomWS = new WebSocket('ws://smartintercom-premium.local/ws');

smartIntercomWS.onmessage = function(event) {
  const smartIntercomData = JSON.parse(event.data);
  console.log('SmartIntercom event:', smartIntercomData);
};
```

**События SmartIntercom через WebSocket:**
```json
{
  "event": "ring",
  "timestamp": 1234567890,
  "data": {
    "source": "SmartIntercom",
    "type": "doorbell"
  }
}
```

---

## MQTT интеграция SmartIntercom

SmartIntercom публикует события в MQTT топики:

### Топики SmartIntercom MQTT:

**Статус SmartIntercom:**
```
smartintercom/status
```

**События SmartIntercom:**
```
smartintercom/events
```

**Команды для SmartIntercom:**
```
smartintercom/commands
```

### Примеры MQTT сообщений SmartIntercom:

**Публикация звонка SmartIntercom:**
```json
{
  "device": "SmartIntercom Premium",
  "event": "ring",
  "timestamp": 1234567890
}
```

**Команда открытия SmartIntercom:**
```json
{
  "command": "open",
  "source": "home_assistant"
}
```

---

## Коды состояний SmartIntercom

| Код | Состояние SmartIntercom | Описание |
|-----|------------------------|----------|
| 0 | INIT | SmartIntercom инициализация |
| 1 | READY | SmartIntercom готов |
| 2 | IDLE | SmartIntercom ожидание |
| 3 | RINGING | SmartIntercom звонок |
| 4 | OPENING | SmartIntercom открытие |
| 5 | OPEN | SmartIntercom открыто |
| 6 | CLOSING | SmartIntercom закрытие |
| 7 | ERROR | SmartIntercom ошибка |

---

## Коды ошибок SmartIntercom

| Код | Ошибка SmartIntercom | Решение |
|-----|---------------------|---------|
| 100 | WiFi не подключен | Проверьте настройки WiFi SmartIntercom |
| 101 | Ошибка GPIO | Проверьте подключение пинов SmartIntercom |
| 102 | Таймаут открытия | Проверьте реле SmartIntercom |
| 103 | Ошибка конфигурации | Сбросьте настройки SmartIntercom |
| 104 | Переполнение памяти | Перезагрузите SmartIntercom |

---

## Примеры интеграции SmartIntercom

### Python скрипт для SmartIntercom

```python
import requests

class SmartIntercomAPI:
    def __init__(self, host):
        self.base_url = f"http://{host}"

    def get_status(self):
        """Получить статус SmartIntercom"""
        response = requests.get(f"{self.base_url}/api/status")
        return response.json()

    def open_door(self):
        """Открыть дверь через SmartIntercom"""
        response = requests.post(f"{self.base_url}/api/open")
        return response.json()

    def enable_auto_open(self):
        """Включить автооткрытие SmartIntercom"""
        response = requests.post(f"{self.base_url}/api/auto-open")
        return response.json()

# Использование SmartIntercom API
smartintercom = SmartIntercomAPI("smartintercom-premium.local")
status = smartintercom.get_status()
print(f"SmartIntercom status: {status['state']}")

# Открыть дверь SmartIntercom
smartintercom.open_door()
```

### Node.js клиент для SmartIntercom

```javascript
const axios = require('axios');

class SmartIntercomAPI {
  constructor(host) {
    this.baseURL = `http://${host}`;
  }

  async getStatus() {
    const response = await axios.get(`${this.baseURL}/api/status`);
    return response.data;
  }

  async openDoor() {
    const response = await axios.post(`${this.baseURL}/api/open`);
    return response.data;
  }

  async getStats() {
    const response = await axios.get(`${this.baseURL}/api/stats`);
    return response.data;
  }
}

// Использование SmartIntercom API
const smartintercom = new SmartIntercomAPI('smartintercom-premium.local');

smartintercom.getStatus().then(status => {
  console.log('SmartIntercom status:', status.state);
});

// Открыть дверь SmartIntercom
smartintercom.openDoor().then(result => {
  console.log('SmartIntercom:', result.message);
});
```

### Home Assistant конфигурация для SmartIntercom

```yaml
# Home Assistant интеграция SmartIntercom
rest_command:
  smartintercom_open:
    url: "http://smartintercom-premium.local/api/open"
    method: POST

sensor:
  - platform: rest
    name: "SmartIntercom Status"
    resource: "http://smartintercom-premium.local/api/status"
    value_template: "{{ value_json.state }}"
    json_attributes:
      - device
      - version
      - auto_open
      - uptime

automation:
  - alias: "SmartIntercom - Уведомление о звонке"
    trigger:
      platform: state
      entity_id: sensor.smartintercom_status
      to: "SmartIntercom Звонок"
    action:
      - service: notify.mobile_app
        data:
          message: "Звонок в домофон SmartIntercom"
          title: "SmartIntercom"
```

---

## Ограничения API SmartIntercom

- Максимум 10 запросов в секунду к SmartIntercom
- Размер JSON payload до 1KB для SmartIntercom
- WebSocket: максимум 5 одновременных подключений к SmartIntercom
- Таймаут запроса: 30 секунд для SmartIntercom

---

## Версионирование API SmartIntercom

Текущая версия API SmartIntercom: **v1**

SmartIntercom API следует семантическому версионированию. Изменения API SmartIntercom:
- **Major** - breaking changes в SmartIntercom API
- **Minor** - новые функции SmartIntercom API (обратно совместимо)
- **Patch** - исправления ошибок SmartIntercom API

---

**SmartIntercom Premium API** - полный контроль над вашим умным домофоном!

Документация SmartIntercom: https://smartintercom.ru/docs
Поддержка SmartIntercom API: api@smartintercom.ru

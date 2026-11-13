# EXPERIMENT DETAILS

**EXP:** CONTINUOUS ASSESSMENT 15 (CAT15)

**DATE:** 14/11/2026

**NAME OF EXP:** Real-Time BME280 Environmental Sensing, Multi-Variable Climate/Altitude Prediction & OpenWeatherMap API Integration

**HARDWARE SETUP / PLATFORM:** STM32 Nucleo-F401RE (Cortex-M4 @ 84 MHz) + Bosch BME280 (I2C1) + ESP8266/Wemos D1 Mini (USART1) + EMQX MQTT Broker + OpenWeatherMap REST API + Node-RED Dashboard

---

## TASK 15 : Real-Time Environmental Sensing, Barometric Altitude Prediction & OpenWeatherMap Cloud Sync

### 1. Hardware Setup & Circuit Diagram

- **Microcontroller:** STM32 Nucleo-F401RE (`STM32F401RET6`, ARM Cortex-M4 @ 84 MHz)
- **Sensor:** Bosch BME280 Combined Temperature, Pressure & Relative Humidity Sensor on `I2C1` (`PB8 = SCL`, `PB9 = SDA`, auto-detected at `0x76` or `0x77`, 3.3V supply)
- **Wi-Fi & MQTT Transport:** ESP8266 / Wemos D1 Mini on `USART1` (`PA9 = TX`, `PA10 = RX`), joined to local Wi-Fi, publishing telemetry to public broker `broker.emqx.io:1883` under topic `college/weather/bme280`
- **Debug / Serial Console:** `USART2` (`PA2/PA3` via onboard ST-Link V2-1 VCP @ 115200 baud)
- **Cloud Reference:** OpenWeatherMap REST API (`api.openweathermap.org/data/2.5/weather`) for regional ground-truth validation
- **Telemetry Visualizer:** Node-RED Interactive Dashboard with real-time gauges, trend charts, dynamic climate synthesis, and calibration delta analysis

![Circuit Diagram](circuit.png)

---

### 2. Core Logic, Algorithms & Code Base

#### Working Principle

1. **Bare-Metal Sensor Acquisition:**
   - The STM32 interrogates `I2C1` to auto-detect the BME280 address (`0x76` with `SDO` tied to GND or `0x77` with `SDO` pulled HIGH).
   - Sensor oversampling is configured to 1x for Temperature, Pressure, and Humidity using the official Bosch `BME280_SensorAPI`.
   - Compensation trim formulas compute physical values: Temperature in °C, Barometric Pressure in hPa (`Pa / 100.0`), and Relative Humidity in %RH.

2. **MQTT Publishing via Custom ESPLib:**
   - The STM32 sends AT commands to configure the ESP8266 in Station Mode, connects to the Wi-Fi network, and opens a TCP socket to `broker.emqx.io:1883`.
   - Formats a compact JSON payload:
     ```json
     {"temperature": 24.33, "pressure": 1012.47, "humidity": 64.75}
     ```
   - Publishes the payload using `MQTT_publishTopic("college/weather/bme280", mqttMessage, QoS0)`.

3. **Node-RED Real-Time Analytics & Prediction Engine:**
   - **Hypsometric Barometric Altitude:**
     $$	ext{Altitude (m)} = 44330 	imes \left(1 - \left(rac{P}{1013.25}ight)^{0.190295}ight)$$
   - **Dynamic Thermal Condition Classification:**
     - $T > 26^\circ	ext{C}$: $	ext{🔥 HOT} \quad (+ \Delta T 	ext{ above } 26^\circ	ext{C})$
     - $T < 24^\circ	ext{C}$: $	ext{❄️ TOO COLD} \quad (- \Delta T 	ext{ below } 24^\circ	ext{C})$
     - $24^\circ	ext{C} \le T \le 26^\circ	ext{C}$: $	ext{🌿 COMFORTABLE} \quad (	ext{Optimum thermal band})$
   - **Altitude Classification:**
     - $P < 900	ext{ hPa}$: $	ext{⛰️ Alpine / Mountain Peak}$
     - $900 \le P < 980	ext{ hPa}$: $	ext{🌲 High Plateau / Hill}$
     - $P \ge 980	ext{ hPa}$: $	ext{🌊 Sea Level / Lowland}$
   - **Moisture Classification:**
     - $H > 70\%$: $	ext{🌧️ High Moisture / Humid}$
     - $H < 40\%$: $	ext{🏜️ Arid / Dry}$
     - $40\% \le H \le 70\%$: $	ext{💧 Comfortable}$

4. **OpenWeatherMap REST API Integration & Ground-Truth Delta:**
   - Node-RED periodically queries `https://api.openweathermap.org/data/2.5/weather?q={city}&appid={API_KEY}&units=metric`.
   - Compares the physical local BME280 sensor reading against the official regional cloud ground truth:
     $$\Delta T = T_{	ext{BME280}} - T_{	ext{Cloud}}, \quad \Delta H = H_{	ext{BME280}} - H_{	ext{Cloud}}, \quad \Delta P = P_{	ext{BME280}} - P_{	ext{Cloud}}$$
   - Detects microclimates, thermal heating effects (e.g. human body proximity, indoor HVAC), and verifies sensor calibration health.

---

### 3. Node-RED Dashboard Configuration & Import

The file [node_red_flow.json](node_red_flow.json) contains the complete ready-to-import flow matching the lab specifications:

1. In Node-RED, open the top-right menu **☰ -> Import**.
2. Select **[node_red_flow.json](node_red_flow.json)** and click **Import**.
3. Open the **"Poll OpenWeatherMap API (5m)"** inject node and enter your free OpenWeatherMap API key into the `apiKey` property (or click **"Simulate Live Cloud Weather (Chennai)"** to test without an API key immediately).
4. Click **Deploy**.
5. Navigate to `http://localhost:1880/ui` to view the live dashboard:
   - **🧠 Dynamic Climate & Altitude Predictions**: Real-time regional synthesis, thermal status, altitude category, and moisture level.
   - **📈 Real-Time Environmental Charts**: Live scrolling historical trends for Temperature, Humidity, and Pressure.
   - **⚡ Live Meters & Current Values**: Analog gauges and large digital displays for instant readings.
   - **☁️ OpenWeatherMap Ground-Truth Sync**: Cloud weather summary and real-time microclimate delta indicators.

---

### 4. Inference

- Physical changes applied to the BME280 sensor (finger touch, warm breath, or enclosure pressure changes) propagate over MQTT and update the Node-RED dashboard in real time with sub-second latency.
- Integrating OpenWeatherMap cloud ground truth provides a practical IoT baseline for comparing localized microclimate indoor environments with regional outdoor conditions and validating embedded sensor calibration.

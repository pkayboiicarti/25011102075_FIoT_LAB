# 25011102075_FIoT_LAB — Foundations of IoT (25CS2741)

## 1. About

This repository contains all laboratory submissions for **25CS2741 — Foundations of Internet of Things + Lab** organized in strict CAT order across **8051 (AT89S52)** bare-metal experiments (`CAT2`–`CAT7`), and **STM32 (Blue Pill STM32F103C8T6 & Nucleo-F401RE)** HAL/IoT experiments (`CAT8`–`CAT15`).

Every task and subtask folder includes its own `README.md` (with Experiment Details, Hardware Setup, Embedded Circuit Diagram `circuit.png`, Pseudocode/Code Base, and Inference) alongside all required Keil µVision / STM32CubeMX source and project files.

---

## 2. Complete CAT Index (CAT2 – CAT15)

| CAT No. | Platform | Experiment Title | Subtasks / Folders |
|---|---|---|---|
| **CAT2** | 8051 (`AT89S52`) | 8051 Tool Chain Setup — Development Board (`AT89S52`) | [TASK2A (Bare-Metal LED Blink)](8051/CAT2/TASK2A/README.md) · [TASK2B (Custom Delay Library)](8051/CAT2/TASK2B/README.md) |
| **CAT3** | 8051 (`AT89S52`) | Timer-Based LED Pattern Generator and Button Interrupt Controller | [TASK3A](8051/CAT3/TASK3A/README.md) |
| **CAT4** | 8051 (`AT89S52`) | Human-Machine Interface using Matrix Keypad, LEDs, and Seven-Segment Display | [TASK4A](8051/CAT4/TASK4A/README.md) |
| **CAT5** | 8051 (`AT89S52`) | Hardware Timer and Interrupt-Based Timing for LED Fading, Pulse Generation, and Stepper Motor Control | [TASK5A (1s Timer0 LED)](8051/CAT5/TASK5A/README.md) · [TASK5B.1 (PWM Fade)](8051/CAT5/TASK5B/TASK5B.1/README.md) · [TASK5B.2 (Variable Freq Square)](8051/CAT5/TASK5B/TASK5B.2/README.md) · [TASK5B.3 (Multi-Waveform)](8051/CAT5/TASK5B/TASK5B.3/README.md) · [TASK5C (Stepper Motor)](8051/CAT5/TASK5C/README.md) |
| **CAT6** | 8051 (`AT89S52`) | UART Driver and Serial Debugging | [TASK6A (UART Init & TX)](8051/CAT6/TASK6A/README.md) · [TASK6B (Interrupt RX Ring-Buffer Echo)](8051/CAT6/TASK6B/README.md) |
| **CAT7** | 8051 (`AT89S52`) | 16x2 LCD Driver Development and Dynamic Display Control | [TASK7A (4-bit Init & Display)](8051/CAT7/TASK7A/README.md) · [TASK7B (Cursor & Numbers)](8051/CAT7/TASK7B/README.md) · [TASK7C (Keypad Movement)](8051/CAT7/TASK7C/README.md) |
| **CAT8** | STM32 (`STM32F103C8T6`) | STM32 GPIO Bring-Up and Switch-Controlled LED | [TASK8A (GPIO I/O)](STM32/CAT8/TASK8A/README.md) · [TASK8B (Event Debounce)](STM32/CAT8/TASK8B/README.md) · [TASK8C (Short/Long Press)](STM32/CAT8/TASK8C/README.md) |
| **CAT9** | STM32 (`STM32F103C8T6`) | STM32 UART Command Controlled LED | [TASK9A (UART Echo)](STM32/CAT9/TASK9A/README.md) · [TASK9B (Command Parser)](STM32/CAT9/TASK9B/README.md) |
| **CAT10** | STM32 (`STM32F103C8T6`) | STM32–Wemos Wi-Fi Modem Interface Using AT Commands | [TASK10A (UART Bridge)](STM32/CAT10/TASK10A/README.md) · [TASK10B (AT Commands)](STM32/CAT10/TASK10B/README.md) · [TASK10C (Wi-Fi Connect)](STM32/CAT10/TASK10C/README.md) |
| **CAT11** | STM32 (`STM32F103C8T6`) | HTTP Server — STM32–Wemos Wi-Fi Modem Using AT Commands | [TASK11A (TCP/HTTP Server)](STM32/CAT11/TASK11A/README.md) · [TASK11B (Node-RED HTTP Client)](STM32/CAT11/TASK11B/README.md) |
| **CAT12** | STM32 (`STM32F401RE`) | STM32F401RE Cortex-M4: GPIO and User Input Programming | [TASK12A (LED Bring-Up)](STM32/CAT12/TASK12A/README.md) · [TASK12B (Switch Memory)](STM32/CAT12/TASK12B/README.md) · [TASK12C (Short/Long Press)](STM32/CAT12/TASK12C/README.md) |
| **CAT13** | STM32 (`STM32F401RE`) | Sensor Interfacing (BME280) — STM32 (Debug) — ESP8266 (Wi-Fi) HTTP — Node Analytics | [TASK13](STM32/CAT13/TASK13/README.md) |
| **CAT14** | STM32 (`STM32F401RE`) | Sensor Interfacing (BME280) — (MQTT) — Node Analytics | [TASK14](STM32/CAT14/TASK14/README.md) |
| **CAT15** | STM32 (`STM32F401RE`) | Real-Time BME280 Environmental Sensing, Multi-Variable Climate/Altitude Prediction & Node-RED Dashboard | [TASK15](STM32/CAT15/TASK15/README.md) |

---

## 3. Repository Directory Structure

```text
25011102075_FIoT_LAB/
├── README.md
├── .gitignore
├── 8051/
│   ├── CAT2/
│   │   ├── TASK2A/  (README.md, circuit.png, *.c, *.uvproj, STARTUP.A51, Objects/*.hex)
│   │   └── TASK2B/
│   ├── CAT3/
│   │   └── TASK3A/
│   ├── CAT4/
│   │   └── TASK4A/
│   ├── CAT5/
│   │   ├── TASK5A/
│   │   ├── TASK5B/
│   │   │   ├── TASK5B.1/
│   │   │   ├── TASK5B.2/
│   │   │   └── TASK5B.3/
│   │   └── TASK5C/
│   ├── CAT6/
│   │   ├── TASK6A/
│   │   └── TASK6B/
│   └── CAT7/
│       ├── TASK7A/
│       ├── TASK7B/
│       └── TASK7C/
└── STM32/
    ├── CAT8/  (TASK8A, TASK8B, TASK8C)
    ├── CAT9/  (TASK9A, TASK9B)
    ├── CAT10/ (TASK10A, TASK10B, TASK10C)
    ├── CAT11/ (TASK11A, TASK11B)
    ├── CAT12/ (TASK12A, TASK12B, TASK12C)
    ├── CAT13/ (TASK13)
    ├── CAT14/ (TASK14)
    └── CAT15/ (TASK15)
```

---

## 4. Build & Evaluation Compliance

- **8051 Projects:** Built for `AT89S52` in Keil µVision (`*.uvproj`), including `STARTUP.A51`, bare-metal Embedded C (`*.c`), `Objects/*.hex`, `circuit.png`, and `README.md`. *(Rebuild `.uvproj` in Keil C51 locally to regenerate target `.hex` binaries before flashing.)*
- **STM32 Projects:** Built with STM32CubeMX + Keil MDK-ARM (`*.ioc`, `.mxproject`, `Core/`, `Drivers/`, `MDK-ARM/*.uvprojx`, `MDK-ARM/startup_*.s`, `circuit.png`, and `README.md`).
- **Clean Git Tracking:** `.gitignore` excludes intermediate build outputs (`*.obj`, `*.lst`, `*.o`, `*.d`, `*.axf`, `*.map`, `*.elf`, `Listings/`, `Debug/`, `Release/`, `*.uvopt*`, `*.uvgui*`) while retaining `Objects/*.hex` and all project configuration files.


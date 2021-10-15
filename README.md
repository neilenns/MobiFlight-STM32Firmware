# MobiFlight-STM32Firmware

[![Open in Visual Studio Code](https://open.vscode.dev/badges/open-in-vscode.svg)](https://open.vscode.dev/danecreekphotography/MobiFlight-STM32Firmware)

Implementation of the [MobiFlight](www.mobiflight.com) firmware for STM32-based boards.

Currently supported modules:

- Analog input (potentiometers)
- Buttons
- LCD displays based on the HD44780 controller (e.g. [20x4 LCD module for Arduino](https://www.amazon.com/gp/product/B01GPUMP9C/)). Displays must be 1, 2 or 4 rows high and 16
  or 20 columns wide.
- 7-segment displays based on the MAX7219 controller (e.g. [8-digit 7-segment tubes](https://www.amazon.com/gp/product/B086GKV958/)). The DATA and CLK pins specified in MobiFlight are ignored, you must use your board's hardware SPI pins for MOSI (DATA) and SCK (CLK).
- Output (LEDs). PWM pins are supported.
- Servos (e.g. [basic servo motors](https://www.amazon.com/Smraza-Helicopter-Airplane-Control-Arduino/dp/B07L2SF3R4/))

Currently supported STM32 boards:

- [NUCLEO-L476RG](https://www.digikey.com/en/products/detail/stmicroelectronics/NUCLEO-L476RG/5347711?s=N4IgTCBcDaIHIFUDCAZAogeQLQoCwHYA2AJQHEQBdAXyA)

## About the code

This project is an exploration of C++17, mbed OS and PlatformIO to write code for an embedded device. It supports various forms of VSCode development
including remote containers and GitHub Codespaces, and uses GitHub actions for CI/CD.

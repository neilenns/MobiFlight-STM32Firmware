/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "drivers/MAX7219.hpp"
#include "MFModule.hpp"

class MFMAX7219 : MFModule
{
public:
  // The provided pins should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFMAX7219(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN, int submoduleCount = 1, std::string name = "7 Segment Display");
  void Display(uint8_t submodule, char *value, uint8_t points, uint8_t mask);
  void PowerSavingMode(bool state) override;
  void Serialize(std::string *buffer) override;
  void StartTest();
  void StartTest(uint8_t submodule);
  void StopTest();
  void StopTest(uint8_t submodule);

private:
  MAX7219 *_display;
  std::string _name;
  int _submoduleCount;

  ARDUINO_PIN _mosiArduino;
  ARDUINO_PIN _sclkArduino;
  ARDUINO_PIN _csArduino;
};

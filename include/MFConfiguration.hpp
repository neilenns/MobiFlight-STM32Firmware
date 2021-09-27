/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFButton.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "modules/MFMAX7219.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

class MFConfiguration
{
private:
  PinManager pinManager;

public:
  map<ARDUINO_PIN, MFButton *> buttons;
  map<int, MFLcdDisplay *> lcdDisplays;
  map<ARDUINO_PIN, MFMAX7219 *> ledDisplays;
  map<ARDUINO_PIN, MFOutput *> outputs;

  void AddButton(ARDUINO_PIN arduinoPinName, char const *name = "Button");
  void AddLcdDisplay(int address, int rows, int columns, char const *name = "LCD Display");
  void AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount, char const *name = "LED Display");
  void AddOutput(ARDUINO_PIN arduinoPinName, char const *name = "Output");

  void Erase();
  void Load();
  void Save();
  void Serialize();
  void StartTest();
  void StopTest();
};

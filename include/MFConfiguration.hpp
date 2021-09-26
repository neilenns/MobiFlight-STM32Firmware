#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFButton.hpp"
#include "modules/MFMAX7219.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

class MFConfiguration
{
private:
  PinManager pinManager;

public:
  map<ARDUINO_PIN, MFButton *> buttons;
  map<ARDUINO_PIN, MFMAX7219 *> ledDisplays;
  map<ARDUINO_PIN, MFOutput *> outputs;

  void AddButton(ARDUINO_PIN arduinoPinName, char const *name = "Output");
  void AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount, char const *name = "LED Display");
  void AddOutput(ARDUINO_PIN arduinoPinName, char const *name = "Output");
  void Erase();
  void Load();
  void Save();
  void Serialize();
  void StartTest();
  void StopTest();
};

#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFButton.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

class MFConfiguration
{
private:
  PinManager pinManager;

public:
  map<ARDUINO_PIN, MFOutput *> outputs;
  map<ARDUINO_PIN, MFButton *> buttons;

  void AddButton(ARDUINO_PIN arduinoPinName, char const *name = "Output");
  void AddOutput(ARDUINO_PIN arduinoPinName, char const *name = "Output");
  void Erase();
  void Load();
  void Save();
  void Serialize();
};

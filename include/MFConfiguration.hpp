#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFButton.hpp"
#include "modules/MFOutput.hpp"

class MFConfiguration
{
public:
  map<ARDUINO_PIN, MFOutput *> outputs;
  map<ARDUINO_PIN, MFButton *> buttons;

  void Erase();
  void Load();
  void Save();
  void Serialize();
};

#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFOutput.hpp"

class MFConfiguration
{
public:
  map<ARDUINO_PIN, MFOutput> outputs;

  void Erase();
  void Load();
  void Save();
  void Serialize();
};

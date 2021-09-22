#pragma once

#include <map>
#include <mbed.h>

#include "modules/MFOutput.hpp"

class MFConfiguration
{
public:
  map<PinName, MFOutput> outputs;

  void Erase();
  void Load();
  void Save();
};
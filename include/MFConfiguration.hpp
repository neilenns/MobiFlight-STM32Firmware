#pragma once

#include <map>
#include <mbed.h>

#include "modules/MFOutput.hpp"

class MFConfiguration
{
  friend ostream &operator<<(ostream &os, const MFConfiguration &obj);

public:
  map<PinName, MFOutput> outputs;

  void Erase();
  void Load();
  void Save();
};

std::ostream &operator<<(std::ostream &os, const MFConfiguration &obj);

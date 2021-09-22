#pragma once

#include <map>
#include <mbed.h>
#include <optional>
#include "modules/MFModuleTypes.hpp"

class PinManager
{
public:
  PinManager(uint8_t numberOfPins = 10);
  void ClearRegisteredPins();
  bool IsPinRegistered(PinName pin);
  void RegisterPin(PinName pin, MFModuleType type);
  std::optional<PinName> MapArudinoPin(int arduinoPin);

private:
  map<PinName, MFModuleType> *_registeredPins;
};

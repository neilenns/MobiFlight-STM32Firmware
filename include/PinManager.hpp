/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <map>
#include <mbed.h>
#include <optional>

#include "ArduinoTypes.hpp"
#include "modules/MFModuleTypes.hpp"

class PinManager
{
public:
  PinManager(ARDUINO_PIN numberOfPins = 10);
  void ClearRegisteredPins();
  bool IsPinRegistered(ARDUINO_PIN pin);
  void RegisterPin(ARDUINO_PIN pin, MFModuleType type);
  static std::optional<PinName> MapArudinoPin(ARDUINO_PIN arduinoPin);

private:
  map<ARDUINO_PIN, MFModuleType> *_registeredPins;
};

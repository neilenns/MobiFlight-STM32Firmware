/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <map>
#include <optional>
#include <mbed.h>

#include "boards/STM32L476.h"
#include "modules/MFModuleTypes.hpp"
#include "PinManager.hpp"

PinManager::PinManager(uint8_t numberOfPins)
{
  _registeredPins = new map<uint8_t, MFModuleType>;
}

void PinManager::ClearRegisteredPins()
{
  _registeredPins->clear();
}

bool PinManager::IsPinRegistered(ARDUINO_PIN arduinoPinName)
{
  return _registeredPins->count(arduinoPinName) > 0;
}

void PinManager::RegisterPin(ARDUINO_PIN arduinoPinName, MFModuleType type)
{
  (*_registeredPins)[arduinoPinName] = type;
}

std::optional<PinName> PinManager::MapArudinoPin(ARDUINO_PIN arduinoPin)
{
  const auto stm32pin = PinMappings.find(arduinoPin);

  if (stm32pin == PinMappings.end())
  {
    return std::nullopt;
  }

  return stm32pin->second;
}
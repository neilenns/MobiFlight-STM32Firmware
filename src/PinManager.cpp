/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <map>
#include <optional>
#include <mbed.h>

#include "boards/STM32L476.h"
#include "PinManager.hpp"

PinManager::PinManager()
{
}

void PinManager::ClearRegisteredPins()
{
  _registeredPins.clear();
}

bool PinManager::IsPinRegistered(ARDUINO_PIN arduinoPinName)
{
  return _registeredPins[arduinoPinName];
}

void PinManager::RegisterPin(ARDUINO_PIN arduinoPinName)
{
  _registeredPins[arduinoPinName] = 1;
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
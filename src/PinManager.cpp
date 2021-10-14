/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <optional>
#include <mbed.h>

#include "boards/STM32L476.h"
#include "Globals.hpp"
#include "MFCommands.hpp"
#include "PinManager.hpp"

PinManager::PinManager()
{
}

void PinManager::ClearRegisteredPins()
{
  _registeredPins.reset();
}

bool PinManager::IsPinRegistered(ARDUINO_PIN arduinoPinName)
{
  if (arduinoPinName > MAX_PIN_NUMBER)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Invalid pin number.");
    return false;
  }

  return _registeredPins[arduinoPinName];
}

void PinManager::RegisterPin(ARDUINO_PIN arduinoPinName)
{
  if (arduinoPinName > MAX_PIN_NUMBER)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Invalid pin number.");
    return;
  }
  _registeredPins[arduinoPinName] = true;
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
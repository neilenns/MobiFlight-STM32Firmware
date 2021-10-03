/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFStepper.hpp"
#include "PinManager.hpp"

MFStepper::MFStepper(ARDUINO_PIN pin1, ARDUINO_PIN pin2, ARDUINO_PIN pin3, ARDUINO_PIN pin4, std::string name)
{
  _arduinoPin1 = pin1;
  _arduinoPin2 = pin2;
  _arduinoPin3 = pin3;
  _arduinoPin4 = pin4;
  _name = name;

  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32pin1 = PinManager::MapArudinoPin(_arduinoPin1);
  if (!stm32pin1)
  {
    // This should do something smarter
    return;
  }

  std::optional<PinName> stm32pin2 = PinManager::MapArudinoPin(_arduinoPin2);
  if (!stm32pin2)
  {
    // This should do something smarter
    return;
  }

  std::optional<PinName> stm32pin3 = PinManager::MapArudinoPin(_arduinoPin3);
  if (!stm32pin3)
  {
    // This should do something smarter
    return;
  }

  std::optional<PinName> stm32pin4 = PinManager::MapArudinoPin(_arduinoPin4);
  if (!stm32pin4)
  {
    // This should do something smarter
    return;
  }

  _stepper = std::make_shared<ULN2003>(*stm32pin1, *stm32pin2, *stm32pin3, *stm32pin4);
}

uint8_t MFStepper::GetId()
{
  return _arduinoPin1;
}

MFModuleType MFStepper::GetModuleType()
{
  return MFModuleType::kStepper;
}

void MFStepper::PowerSavingMode(bool state)
{
}

void MFStepper::Serialize(std::string &buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer.append(fmt::format("{}.{}.{}.{}.{}.{}:", MFModuleType::kStepper, _arduinoPin1, _arduinoPin2, _arduinoPin3, _arduinoPin4, _name));
}

void MFStepper::StartTest()
{
}

void MFStepper::StopTest()
{
}
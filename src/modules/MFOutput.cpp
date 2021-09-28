/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

MFOutput::MFOutput(ARDUINO_PIN arduinoPinName, const std::string &name)
{
  _arduinoPinName = arduinoPinName;

  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32pin = PinManager::MapArudinoPin(arduinoPinName);
  if (!stm32pin)
  {
    // This should do something smarter
    return;
  }
  _pin = new DigitalOut(*stm32pin);

  _name = name;
  _value = false;
  set(_value);
}

uint8_t MFOutput::get()
{
  return _value;
}

uint8_t MFOutput::GetId()
{
  return _arduinoPinName;
}

MFModuleType MFOutput::GetModuleType()
{
  return MFModuleType::kOutput;
}

void MFOutput::PowerSavingMode(bool state)
{
  state ? _pin->write(0) : _pin->write(_value);
}

void MFOutput::set(uint8_t value)
{
  _value = value;
  _pin->write(value);
}

void MFOutput::Serialize(std::string *buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer->append(fmt::format("{}.{}.{}:", MFModuleType::kOutput, _arduinoPinName, _name));
}

void MFOutput::StartTest()
{
  _pin->write(1);
}

void MFOutput::StopTest()
{
  _pin->write(0);
}
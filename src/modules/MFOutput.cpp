/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

MFOutput::MFOutput(ARDUINO_PIN arduinoPinName, std::string name)
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

void MFOutput::PowerSavingMode(bool state)
{
  state ? set(0) : set(_value);
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
/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>
#include <PeripheralPins.h>
#include <pinmap.h>

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

  // If the pin is a PWM-enabled pin then create a PWM output for it.
  // There is zero documentation for the pinmap_find_function() method.
  // The only information I could find is the source code at
  // https://github.dev/ARMmbed/mbed-os/blob/a3be10c976c36da222517abc0cb4f81e88ff8552/hal/source/mbed_pinmap_common.c#L82
  // It's important to cast this to PinName otherwise the comparison to NC
  // won't work. The function returns NC if the requested pin isn't in the pin map.
  auto function = (PinName)pinmap_find_function(*stm32pin, PinMap_PWM);

  if (function == PinName::NC)
  {
    _digitalPin = std::make_unique<DigitalOut>(*stm32pin);
  }
  else
  {
    _pwmPin = std::make_unique<PwmOut>(*stm32pin);
  }

  _name = name;
  set(0); // Turn the LED off by default
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
  if (_digitalPin)
  {
    state ? _digitalPin->write(0) : _digitalPin->write(_value);
  }
  else
  {
    state ? _pwmPin->write(0) : _pwmPin->write(_value);
  }
}

void MFOutput::set(uint8_t value)
{
  _value = value;
  _digitalPin ? _digitalPin->write(_value) : _pwmPin->write(_value);
}

void MFOutput::Serialize(std::string *buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer->append(fmt::format("{}.{}.{}:", MFModuleType::kOutput, _arduinoPinName, _name));
}

void MFOutput::StartTest()
{
  _digitalPin ? _digitalPin->write(1) : _pwmPin->write(1);
}

void MFOutput::StopTest()
{
  _digitalPin ? _digitalPin->write(1) : _pwmPin->write(1);
}
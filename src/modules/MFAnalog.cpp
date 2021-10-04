/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>
#include <PeripheralPins.h>

#include "ArduinoTypes.hpp"
#include "Globals.hpp"
#include "MFCommands.hpp"
#include "modules/MFAnalog.hpp"

// mbed os returns analog values as a float between 0 and 1.0.
// Arduino expects an integer precision between 0 and 4095 (for 12-bit ADC).
#define ANALOG_PRECISION 4095

// How often the ticker fires to check for an update to the analog
// input value.
#define ANALOG_UPDATE_FREQUENCY 1000ms

MFAnalog::MFAnalog(ARDUINO_PIN arduinoPinName, int sensitivity, std::string name)
{
  _arduinoPinName = arduinoPinName;

  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32pin = PinManager::MapArudinoPin(arduinoPinName);
  if (!stm32pin)
  {
    // This should do something smarter
    return;
  }

  // Check and make sure the pin supports analog input.
  // There is zero documentation for the pinmap_find_function() method.
  // The only information I could find is the source code at
  // https://github.dev/ARMmbed/mbed-os/blob/a3be10c976c36da222517abc0cb4f81e88ff8552/hal/source/mbed_pinmap_common.c#L82
  // It's important to cast this to PinName otherwise the comparison to NC
  // won't work. The function returns NC if the requested pin isn't in the pin map.
  auto function = (PinName)pinmap_find_function(*stm32pin, PinMap_ADC);

  if (function == PinName::NC)
  {
    // This should do something smarter
    return;
  }

  _sensitivity = sensitivity;
  _name = name;
  _value = 0;

  _pin = std::make_shared<AnalogIn>(*stm32pin);
  _ticker.attach(queue->event(callback(this, &MFAnalog::ReadCurrentValue)), ANALOG_UPDATE_FREQUENCY);
}

uint8_t MFAnalog::GetId()
{
  return _arduinoPinName;
}

MFModuleType MFAnalog::GetModuleType()
{
  return MFModuleType::kAnalogInput;
}

void MFAnalog::PowerSavingMode(bool state)
{
}

void MFAnalog::ReadCurrentValue()
{
  if (!_pin)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Attempted to read the analog value on a pin that didn't get created properly.");
    return;
  }

  // Use the analog float reading as a percentage to apply to the Arduino-style
  // integer value.
  auto currentValue = static_cast<int>(ANALOG_PRECISION * _pin->read());
  if (abs(currentValue - _value) >= _sensitivity)
  {
    _value = currentValue;
    cmdMessenger.sendCmdStart(MFCommand::kAnalogChange);
    cmdMessenger.sendCmdArg(_name);
    cmdMessenger.sendCmdArg(_value);
    cmdMessenger.sendCmdEnd();
  }
}

void MFAnalog::Serialize(std::string &buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer.append(fmt::format("{}.{}.{}.{}:", as_integer(MFModuleType::kAnalogInput), _arduinoPinName, _sensitivity, _name));
}

void MFAnalog::StartTest()
{
}

void MFAnalog::StopTest()
{
}
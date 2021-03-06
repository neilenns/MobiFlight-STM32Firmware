/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>
#include <PeripheralPins.h>

#include "ArduinoTypes.hpp"
#include "modules/MFOutput.hpp"
#include "PinManager.hpp"

MFOutput::MFOutput(ARDUINO_PIN arduinoPinName, std::string name)
{
  _arduinoPinName = arduinoPinName;
  _name = name;

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

  if (function != PinName::NC)
  {
    _pwmPin = std::make_shared<PwmOut>(*stm32pin);
  }
  else
  {
    _digitalPin = std::make_shared<DigitalOut>(*stm32pin);
  }

  // Turn the LED off by default
  set(0);
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
  if (_pwmPin)
  {
    state ? _pwmPin->write(0) : _pwmPin->write(CONVERT_TO_MBED_PWM_VALUE(_value));
  }
  else
  {
    state ? _digitalPin->write(0) : _digitalPin->write(_value);
  }
}

void MFOutput::set(uint8_t value)
{
  _value = value;
  _pwmPin ? _pwmPin->write(CONVERT_TO_MBED_PWM_VALUE(_value)) : _digitalPin->write(_value);
}

void MFOutput::Serialize(std::string &buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer.append(fmt::format("{}.{}.{}:", MFModuleType::kOutput, _arduinoPinName, _name));
}

void MFOutput::StartTest()
{
  // The PWM test starts at max brightness then ramps down to 0.
  if (_pwmPin)
  {
    for (int i = 255; i >= 0; i--)
    {
      _pwmPin->write(CONVERT_TO_MBED_PWM_VALUE(i));
      ThisThread::sleep_for(5ms);
    }
  }
  else
  {
    _digitalPin->write(1);
  }
}

void MFOutput::StopTest()
{
  _pwmPin ? _pwmPin->write(CONVERT_TO_MBED_PWM_VALUE(_value)) : _digitalPin->write(_value);
}
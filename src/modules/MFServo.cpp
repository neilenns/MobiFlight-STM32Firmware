/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>
#include <PeripheralPins.h>

#include "ArduinoTypes.hpp"
#include "modules/MFServo.hpp"
#include "PinManager.hpp"

MFServo::MFServo(ARDUINO_PIN arduinoPinName, const std::string &name)
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

  _servo = std::make_shared<Servo>(*stm32pin);

  // Turn the LED off by default
  _value = 0;
  set(_value);
}

uint8_t MFServo::get()
{
  return _value;
}

uint8_t MFServo::GetId()
{
  return _arduinoPinName;
}

MFModuleType MFServo::GetModuleType()
{
  return MFModuleType::kServo;
}

void MFServo::PowerSavingMode(bool state)
{
}

void MFServo::set(uint8_t value)
{
  _servo->write(value);
}

void MFServo::Serialize(std::string *buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer->append(fmt::format("{}.{}.{}:", MFModuleType::kServo, _arduinoPinName, _name));
}

void MFServo::StartTest()
{
  for (int i = 0; i < 100; i++)
  {
    _servo->write(i / 100.0);
    ThisThread::sleep_for(1ms);
  }
  for (int i = 100; i > 0; i--)
  {
    _servo->write(i / 100.0);
    ThisThread::sleep_for(1ms);
  }
}

void MFServo::StopTest()
{
}
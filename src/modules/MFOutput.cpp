// MFOutput.cpp
//
// Copyright (C) 2013-2014

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

void MFOutput::powerSavingMode(bool state)
{
  state ? set(0) : set(_value);
}

void MFOutput::set(uint8_t value)
{
  _value = value;
  _pin->write(value);
}

// Example of what good output looks like:
// 10,8.16.15.0.Encoder:1.14.Button:;
std::ostream &operator<<(std::ostream &os, const MFOutput &obj)
{
  // The + on _arduinoPinName converts it from a "uint8_t" (which << sees as an unsigned char)
  // to a signed number that gets printed out as an actual number. Otherwise it spits out the
  // ASCII character for that value which is... not right.
  os << as_integer(MFModuleType::kOutput) << "." << +obj._arduinoPinName << "." << obj._name;
  return os;
}

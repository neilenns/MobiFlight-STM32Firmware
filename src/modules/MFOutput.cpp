// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>
#include "modules/MFOutput.hpp"

MFOutput::MFOutput(PinName pinName, std::string name)
{
  _pin = new DigitalOut(pinName);
  _pinName = pinName;
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
  os << as_integer(MFModuleType::kOutput) << "." << obj._pinName << "." << obj._name << ":";
  return os;
}

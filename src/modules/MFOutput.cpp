// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "modules/MFOutput.hpp"

MFOutput::MFOutput(PinName pin)
{
  _pin = new DigitalOut(pin);
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

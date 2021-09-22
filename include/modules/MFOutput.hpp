#pragma once

#include <mbed.h>
#include <iostream>
#include "MFModuleTypes.hpp"

class MFOutput
{
public:
  MFOutput(PinName pin = PA_0, std::string name = "LED");
  uint8_t get();
  void powerSavingMode(bool state);
  void set(uint8_t value);
  friend ostream &operator<<(ostream &os, const MFOutput &obj);

private:
  PinName _pinName;
  DigitalOut *_pin;
  uint8_t _value;
  std::string _name;
};

std::ostream &operator<<(std::ostream &os, const MFOutput &obj);

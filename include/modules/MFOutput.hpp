#pragma once

#include <mbed.h>

class MFOutput
{
public:
  MFOutput(PinName pin = PA_0);
  uint8_t get();
  void powerSavingMode(bool state);
  void set(uint8_t value);

private:
  DigitalOut *_pin;
  uint8_t _value;
};

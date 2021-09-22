#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "MFModuleTypes.hpp"

class MFOutput
{
public:
  // The provided pin should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFOutput(ARDUINO_PIN pin = 0, std::string name = "LED");
  uint8_t get();
  void powerSavingMode(bool state);
  void set(uint8_t value);
  friend ostream &operator<<(ostream &os, const MFOutput &obj);

private:
  uint8_t _arduinoPinName;
  DigitalOut *_pin;
  uint8_t _value;
  std::string _name;
};

std::ostream &operator<<(std::ostream &os, const MFOutput &obj);

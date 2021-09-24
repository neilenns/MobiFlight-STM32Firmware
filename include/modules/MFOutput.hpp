#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "MFModuleTypes.hpp"

class MFOutput final
{
public:
  // The provided pin should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFOutput(ARDUINO_PIN pin = 0, std::string name = "LED");
  uint8_t get();
  void PowerSavingMode(bool state);
  void Serialize(char *str, size_t len);
  void set(uint8_t value);

private:
  uint8_t _arduinoPinName;
  DigitalOut *_pin;
  uint8_t _value;
  std::string _name;
};

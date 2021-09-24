#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "drivers/MAX7219.hpp"

class MFMAX7219 final
{
public:
  // The provided pin should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFMAX7219(ARDUINO_PIN mosi = 1, ARDUINO_PIN sclk = 2, ARDUINO_PIN cs = 3, std::string name = "7 Segment Display");
  void Display(uint8_t submodule, char *value, uint8_t points, uint8_t mask);
  void Serialize(char *str, size_t len);
  void StartTest();
  void StopTest();

private:
  MAX7219 *_display;
  ARDUINO_PIN _mosiArduino;
  ARDUINO_PIN _sclkArduino;
  ARDUINO_PIN _csArduino;
  std::string _name;
};

#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "DebounceIn.hpp"
#include "MFModuleTypes.hpp"

class MFButton final
{
public:
  // The provided pin should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFButton(ARDUINO_PIN pin = 0, std::string name = "Button");
  void Serialize(char *str, size_t len);
  void OnPress();
  void OnRelease();

private:
  uint8_t _arduinoPinName;
  DebounceIn *_pin;
  DigitalOut *_led;
  std::string _name;
};

enum MFButtonState
{
  Pressed,
  Released,
};
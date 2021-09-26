#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "drivers/TextLCD.h"

class MFLcdDisplay final
{
public:
  // The provided pins should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFLcdDisplay(char deviceAddress, TextLCD_Base::LCDType deviceType, std::string name = "LCD Display");
  void Display(char *text);
  void Serialize(char *str, size_t len);
  void StartTest();
  void StopTest();

private:
  TextLCD_I2C *_display;
  I2C *_i2c_lcd;

  char _deviceAddress;
  TextLCD_Base::LCDType _deviceType;
  std::string _name;
};

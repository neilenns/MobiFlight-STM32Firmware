#pragma once

#include <iostream>
#include <TextLCD.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"

class MFLcdDisplay final
{
public:
  MFLcdDisplay(char deviceAddress, int rows, int columns, std::string name = "LCD Display");
  void Display(const char *text);
  void PowerSavingMode(bool state);
  void Serialize(char *str, size_t len);
  void StartTest();
  void StopTest();

private:
  TextLCD_I2C *_display;
  I2C *_i2c_lcd;

  int _columns;
  char _deviceAddress;
  std::string _name;
  int _rows;

  static std::optional<TextLCD_Base::LCDType> LookupDeviceType(int rows, int columns);
  void PrintCentered(const char *text, int line);
};

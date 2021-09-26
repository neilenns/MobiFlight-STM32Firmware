// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "PinManager.hpp"

MFLcdDisplay::MFLcdDisplay(char deviceAddress, TextLCD_Base::LCDType deviceType, std::string name)
{
  _deviceType = TextLCD_Base::LCDType::LCD20x4;
  _name = name;
  // mbed os uses 8-bit addressing, but MobiFlight uses 7-bit.
  // Shift left once to convert the Arduino address to mbed os address.
  _deviceAddress = deviceAddress << 1;

  _i2c_lcd = new I2C(I2C_SDA, I2C_SCL);
  _display = new TextLCD_I2C(_i2c_lcd, _deviceAddress, _deviceType);
}

void MFLcdDisplay::Display(uint8_t submodule, char *value, uint8_t points, uint8_t mask)
{
}

/**
 * @brief Writes the configuraton to a string.
 * 
 * @param str Buffer to write the configuration to.
 * @param len Length of the buffer.
 */
void MFLcdDisplay::Serialize(char *str, size_t len)
{
  snprintf(str, len, "%i.%2x.%i.%s", as_integer(MFModuleType::kLcdDisplayI2C), _deviceAddress, _deviceType, _name.c_str());
  str[len - 1] = '\0';
}

/**
 * @brief Turns on test mode for the LCD display.
 */
void MFLcdDisplay::StartTest()
{
  _display->setBacklight(TextLCD_Base::LCDBacklight::LightOn);
  _display->setCursor(TextLCD::CurOff_BlkOn);
  _display->printf("Mobiflight rocks!!!");
}

/**
 * @brief Turns off test mode for the LCD display.
 */
void MFLcdDisplay::StopTest()
{
  _display->setCursor(TextLCD::CurOff_BlkOff);
}
// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "PinManager.hpp"

MFLcdDisplay::MFLcdDisplay(char deviceAddress, TextLCD_Base::LCDType deviceType, std::string name)
{
  _deviceType = deviceType;
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
  printf("LCD Test. Columns=%d, Rows=%d\n\r", _display->columns(), _display->rows());

  for (int row = 0; row < _display->rows(); row++)
  {
    int col = 0;

    printf("MemAddr(Col=%d, Row=%d)=0x%02X\n\r", col, row, _display->getAddress(col, row));
    _display->putc('0' + row);

    for (col = 1; col < _display->columns() - 1; col++)
    {
      _display->putc('*');
    }

    printf("MemAddr(Col=%d, Row=%d)=0x%02X\n\r", col, row, _display->getAddress(col, row));
    _display->putc('+');
  }
}

/**
 * @brief Turns off test mode for the LCD display.
 */
void MFLcdDisplay::StopTest()
{
}
// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "PinManager.hpp"

/**
 * @brief Construct a new MFLcdDisplay::MFLcdDisplay object
 * 
 * @param deviceAddress The 7-bit I2C address of the display
 * @param rows The number of rows on the display. Valid values are 1, 2, and 4.
 * @param columns The number of columns on the display. Valid values are 16 and 20.
 * @param name The name of the display.
 */
MFLcdDisplay::MFLcdDisplay(char deviceAddress, int rows, int columns, std::string name)
{
  // TODO: Handle this bounds checking better better.
  if (rows != 1 && rows != 2 && rows != 4)
  {
    return;
  }

  if (columns != 16 && columns != 20)
  {
    return;
  }

  auto displayType = MFLcdDisplay::LookupDeviceType(rows, columns);

  // TODO: Handle this better.
  if (!displayType)
  {
    return;
  }

  _columns = columns;
  _deviceAddress = deviceAddress;
  _name = name;
  _rows = rows;

  _i2c_lcd = new I2C(I2C_SDA, I2C_SCL);
  // mbed os uses 8-bit addressing, but MobiFlight uses 7-bit.
  // Shift left once to convert the MobiFlight address to mbed os address.
  _display = new TextLCD_I2C(_i2c_lcd, _deviceAddress << 1, *displayType);
  _display->setMode(TextLCD_Base::LCDMode::DispOn);
  _display->setBacklight(TextLCD_Base::LCDBacklight::LightOn);
  _display->setCursor(TextLCD::CurOff_BlkOff);
  StartTest();
}

/**
 * @brief Displays a string of text on the display, wrapping it to the number
 * of available rows on the display.
 * 
 * @param text The text to display.
 */
void MFLcdDisplay::Display(const std::string text)
{
  // Keeps track of the current position in the string when splitting it into
  // multiple lines for display. This is used to ensure that substr() isn't
  // called with a position beyond the length of the string being displayed.
  size_t currentPosition = 0;

  // The text comes in as a long string and needs to get split up into rows
  // of text the length of each row.
  for (auto row = 0; row < _rows && currentPosition < text.length(); row++)
  {
    _display->locate(0, row);
    _display->printf((text.substr(currentPosition, _columns)).c_str());
    currentPosition += _columns;
  }
}

/**
 * @brief Returns the LCDType for a display given a row and column count.
 * 
 * @param rows The number of rows on the display.
 * @param columns The number of columns on the display.
 * @return std::optional<TextLCD_Base::LCDType> The matching LCDType, or null if not supported.
 */
std::optional<TextLCD_Base::LCDType> MFLcdDisplay::LookupDeviceType(int rows, int columns)
{
  // This is really dumb. There has to be a better way to do this.
  if (rows == 1)
  {
    if (columns == 16)
    {
      return TextLCD_Base::LCD16x1;
    }
    else if (columns == 20)
    {
      return TextLCD_Base::LCD20x1;
    }
    else
    {
      return std::nullopt;
    }
  }
  else if (rows == 2)
  {
    if (columns == 16)
    {
      return TextLCD_Base::LCD16x2;
    }
    else if (columns == 20)
    {
      return TextLCD_Base::LCD20x2;
    }
    else
    {
      return std::nullopt;
    }
  }
  else if (rows == 4)
  {
    if (columns == 16)
    {
      return TextLCD_Base::LCD16x4;
    }
    else if (columns == 20)
    {
      return TextLCD_Base::LCD20x4;
    }
    else
    {
      return std::nullopt;
    }
  }
  else
  {
    return std::nullopt;
  }
}

/**
 * @brief Enables or disables power saving mode.
 * 
 * @param state True to enable power saving mode, false to disable it.
 */
void MFLcdDisplay::PowerSavingMode(bool state)
{
  if (state)
  {
    _display->setBacklight(TextLCD_Base::LCDBacklight::LightOff);
    _display->setMode(TextLCD_Base::LCDMode::DispOff);
  }
  else
  {
    _display->setBacklight(TextLCD_Base::LCDBacklight::LightOn);
    _display->setMode(TextLCD_Base::LCDMode::DispOn);
  }
}

/**
 * @brief Prints text centered horizontally on the display.
 * 
 * @param text The text to display.
 * @param row The row to display the text on (origin 0).
 */
void MFLcdDisplay::PrintCentered(const char *text, int row)
{
  if (row < 0 || row > _rows)
  {
    return;
  }

  _display->locate(0, row);

  for (size_t c = 0; c != ((_columns - strlen(text)) / 2); c++)
  {
    _display->printf(" ");
  }

  _display->printf(text);

  for (size_t c = 0; c != ((_columns - strlen(text)) / 2); c++)
  {
    _display->printf(" ");
  }
}

/**
 * @brief Writes the configuraton to a string.
 * 
 * @param str Buffer to write the configuration to.
 * @param len Length of the buffer.
 */
void MFLcdDisplay::Serialize(char *str, size_t len)
{
  // MobiFlight expects the number of columns before the number of rows.
  snprintf(str, len, "%i.%i.%i.%i.%s", as_integer(MFModuleType::kLcdDisplayI2C), _deviceAddress, _columns, _rows, _name.c_str());
  str[len - 1] = '\0';
}

/**
 * @brief Turns on test mode for the LCD display.
 */
void MFLcdDisplay::StartTest()
{
  int preLines = 0;
  _display->cls();

  if (_rows > 2)
  {
    preLines = floor(_rows / 2) - 1;
  }

  PrintCentered("MobiFlight", preLines++);
  if (_rows > 1)
  {
    PrintCentered("Rocks!", preLines++);
  }

  _display->locate(0, 0);
}

/**
 * @brief Turns off test mode for the LCD display.
 */
void MFLcdDisplay::StopTest()
{
  _display->cls();
}
/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "PinManager.hpp"

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

  _i2c_lcd = std::make_shared<I2C>(I2C_SDA, I2C_SCL);
  // mbed os uses 8-bit addressing, but MobiFlight uses 7-bit.
  // Shift left once to convert the MobiFlight address to mbed os address.
  _display = std::make_shared<TextLCD_I2C>(_i2c_lcd.get(), _deviceAddress << 1, *displayType);
  _display->setMode(TextLCD_Base::LCDMode::DispOn);
  _display->setBacklight(TextLCD_Base::LCDBacklight::LightOn);
  _display->setCursor(TextLCD::CurOff_BlkOff);
  StartTest();
}

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
uint8_t MFLcdDisplay::GetId()
{
  return _deviceAddress;
}

MFModuleType MFLcdDisplay::GetModuleType()
{
  return MFModuleType::kLcdDisplayI2C;
}

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

void MFLcdDisplay::Serialize(std::string &buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  // MobiFlight expects the number of columns before the number of rows.
  buffer.append(fmt::format("{}.{:d}.{}.{}:", MFModuleType::kLcdDisplayI2C, _deviceAddress, _columns, _rows));
}

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

void MFLcdDisplay::StopTest()
{
  _display->cls();
}
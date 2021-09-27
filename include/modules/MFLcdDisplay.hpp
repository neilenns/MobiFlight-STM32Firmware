/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <TextLCD.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"

class MFLcdDisplay final
{
public:
  /**
 * @brief Construct a new MFLcdDisplay::MFLcdDisplay object
 * 
 * @param deviceAddress The 7-bit I2C address of the display
 * @param rows The number of rows on the display. Valid values are 1, 2, and 4.
 * @param columns The number of columns on the display. Valid values are 16 and 20.
 * @param name The name of the display.
 */
  MFLcdDisplay(char deviceAddress, int rows, int columns, std::string name = "LCD Display");

  /**
 * @brief Displays a string of text on the display, wrapping it to the number
 * of available rows on the display.
 * 
 * @param text The text to display.
 */
  void Display(const std::string text);

  /**
 * @brief Enables or disables power saving mode.
 * 
 * @param state True to enable power saving mode, false to disable it.
 */
  void PowerSavingMode(bool state);

  /**
 * @brief Writes the configuraton to a string.
 * 
 * @param str Buffer to write the configuration to.
 * @param len Length of the buffer.
 */
  void Serialize(std::string *buffer);

  /**
 * @brief Turns on test mode for the LCD display.
 */
  void StartTest();

  /**
 * @brief Turns off test mode for the LCD display.
 */
  void StopTest();

private:
  int _columns;
  char _deviceAddress;
  TextLCD_I2C *_display;
  I2C *_i2c_lcd;
  std::string _name;
  int _rows;

  /**
 * @brief Returns the LCDType for a display given a row and column count.
 * 
 * @param rows The number of rows on the display.
 * @param columns The number of columns on the display.
 * @return std::optional<TextLCD_Base::LCDType> The matching LCDType, or null if not supported.
 */
  static std::optional<TextLCD_Base::LCDType> LookupDeviceType(int rows, int columns);

  /**
 * @brief Prints text centered horizontally on the display.
 * 
 * @param text The text to display.
 * @param row The row to display the text on (origin 0).
 */
  void PrintCentered(const char *text, int line);
};

/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "drivers/MAX7219.hpp"
#include "MFModule.hpp"

class MFMAX7219 : MFModule
{
public:
  // Even though this takes an Arduino pin value for mosi, sclk, and cs, the only one
  // used interally is cs. mosi and sclk are always the SPI_MOSI and SPI_SCK
  // pins on the STM32. All three are still accepted as parameters though as that's
  // how MobiFlight saves/loads them. If all three weren't kept then the config wouldn't
  // make sense when MobiFlight talks to the STM32.
  /**
 * @brief Construct a new MFMAX7219::MFMAX7219 object
 * 
 * @param mosi The Arduino pin number used for MOSI. Note that in this firmware the hardware SPI_MOSI pin will always be used.
 * @param sclk The Arduino pin number used for SCLK. Note that in this firmware the hardware SPI_SCK pin will always be used.
 * @param cs The Arduino pin number used for CS. This is also the device's unique ID.
 * @param submoduleCount The number of connected submodules.
 * @param name The name for this module.
 */
  MFMAX7219(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount = 1, std::string name = "7 Segment Display");

  /**
 * @brief Displays the specified string on the connected display.
 * 
 * @param submodule The submodule to display the string on.
 * @param value The value to display.
 * @param points A bit array specifying which decimal positions should be illuminated.
 * @param mask A bit array specifying which digits from the value should be displayed.
 */
  void Display(uint8_t submodule, char *value, uint8_t points, uint8_t mask);

  /**
   * @brief Displays test data.
   * 
   * @param submodule The submodule to display the data on.
   */
  void StartTest(uint8_t submodule);

  /**
   * @brief Stops displaying test data, returning the disply to blank values.
   * 
   * @param submodule The submodule to stop displaying test data on.
   */
  void StopTest(uint8_t submodule);

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string *buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  MAX7219 *_display;
  std::string _name;
  int _submoduleCount;

  ARDUINO_PIN _mosiArduino;
  ARDUINO_PIN _sclkArduino;
  ARDUINO_PIN _csArduino;
};

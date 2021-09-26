// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFMAX7219.hpp"
#include "PinManager.hpp"

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
 * @param cs The Arduino pin number used for CS.
 * @param submoduleCount The number of connected submodules.
 * @param name The name for this conneted module.
 */
MFMAX7219::MFMAX7219(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount, std::string name)
{
  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32cs = PinManager::MapArudinoPin(cs);

  if (!stm32cs)
  {
    // This should do something smarter
    return;
  }

  _mosiArduino = mosi;
  _sclkArduino = sclk;
  _csArduino = cs;
  _submoduleCount = submoduleCount;
  _name = name;

  _display = new MAX7219(SPI_MOSI, SPI_SCK, *stm32cs, _submoduleCount);
}

/**
 * @brief Displays the specified string on the connected display.
 * 
 * @param submodule The submodule to display the string on.
 * @param value The value to display.
 * @param points A bit array specifying which decimal positions should be illuminated.
 * @param mask A bit array specifying which digits from the value should be displayed.
 */
void MFMAX7219::Display(uint8_t submodule, char *value, uint8_t points, uint8_t mask)
{
  uint8_t digit = 8;
  uint8_t pos = 0;
  for (int i = 0; i != 8; i++)
  {
    digit--;
    if (((1 << digit) & mask) == 0)
      continue;
    _display->DisplayChar(digit, value[pos], ((1 << digit) & points), submodule);
    pos++;
  }
}

/**
 * @brief Writes the configuraton to a string.
 * 
 * @param str Buffer to write the configuration to.
 * @param len Length of the buffer.
 */
void MFMAX7219::Serialize(char *str, size_t len)
{
  snprintf(str, len, "%i.%i.%i.%i.%s", as_integer(MFModuleType::kLedSegment), _mosiArduino, _sclkArduino, _csArduino, _name.c_str());
  str[len - 1] = '\0';
}

/**
 * @brief Turns on test mode for all connected submodules.
 */
void MFMAX7219::StartTest()
{
  for (int i = 0; i < _submoduleCount; i++)
  {
    StartTest(i);
  }
}

/**
 * @brief Turns on test mode for the specified submodule.
 * @param submodule The submodule to start test mode on.
 */
void MFMAX7219::StartTest(uint8_t submodule)
{
  _display->MAX7219_DisplayTestStart(submodule);
}

/**
 * @brief Turns off test mode for the specified submodule.
 * @param submodule The submodule to stop test mode on.
 */
void MFMAX7219::StopTest()
{
  for (int i = 0; i < _submoduleCount; i++)
  {
    StopTest(i);
  }
}

/**
 * @brief Turns off test mode for all connected submodules.
 */
void MFMAX7219::StopTest(uint8_t submodule)
{
  _display->MAX7219_DisplayTestStop(submodule);
}
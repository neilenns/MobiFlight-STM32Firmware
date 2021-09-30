/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFMAX7219.hpp"
#include "PinManager.hpp"

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

  _display = std::make_shared<MAX7219>(SPI_MOSI, SPI_SCK, *stm32cs, _submoduleCount);
}

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

uint8_t MFMAX7219::GetId()
{
  return _mosiArduino;
}

MFModuleType MFMAX7219::GetModuleType()
{
  return MFModuleType::kLedSegment;
}

void MFMAX7219::PowerSavingMode(bool state)
{
}

void MFMAX7219::Serialize(std::string &buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer.append(fmt::format("{}.{}.{}.{}.{}:", MFModuleType::kLedSegment, _mosiArduino, _sclkArduino, _csArduino, _name));
}

void MFMAX7219::StartTest()
{
  for (int i = 0; i < _submoduleCount; i++)
  {
    StartTest(i);
  }
}

void MFMAX7219::StartTest(uint8_t submodule)
{
  _display->MAX7219_DisplayTestStart(submodule);
}

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
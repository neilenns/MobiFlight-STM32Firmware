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

void MFMAX7219::Serialize(char *str, size_t len)
{
  snprintf(str, len, "%i.%i.%i.%i.%s", as_integer(MFModuleType::kLedSegment), _mosiArduino, _sclkArduino, _csArduino, _name.c_str());
  str[len - 1] = '\0';
}

void MFMAX7219::StartTest()
{
  _display->MAX7219_DisplayTestStart(1);
}

void MFMAX7219::StopTest()
{
  _display->MAX7219_DisplayTestStop(1);
}
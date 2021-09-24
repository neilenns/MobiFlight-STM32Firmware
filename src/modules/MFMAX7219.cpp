// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFMAX7219.hpp"
#include "PinManager.hpp"

MFMAX7219::MFMAX7219(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, std::string name)
{
  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32mosi = PinManager::MapArudinoPin(mosi);
  std::optional<PinName> stm32sclk = PinManager::MapArudinoPin(sclk);
  std::optional<PinName> stm32cs = PinManager::MapArudinoPin(cs);

  if (!stm32mosi || !stm32sclk || !stm32cs)
  {
    // This should do something smarter
    return;
  }

  _display = new MAX7219(*stm32mosi, *stm32sclk, *stm32cs);

  _mosiArduino = mosi;
  _sclkArduino = sclk;
  _csArduino = cs;

  _name = name;
}

// Example of what good output looks like:
// 10,8.16.15.0.Encoder:1.14.Button:;
void MFMAX7219::Serialize(char *str, size_t len)
{
  snprintf(str, len, "%i.%i.%i.%i.%s", as_integer(MFModuleType::kLedSegment), _mosiArduino, _sclkArduino, _csArduino, _name.c_str());
  str[len - 1] = '\0';
}

void MFMAX7219::StartTest()
{
  _display->MAX7219_DisplayTestStart();
}

void MFMAX7219::StopTest()
{
  _display->MAX7219_DisplayTestStop();
}
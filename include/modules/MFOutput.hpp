/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "MFModule.hpp"
#include "MFModuleTypes.hpp"

class MFOutput : MFModule
{
public:
  // The provided pin should be an Arduino pin number. This will get mapped
  // internally to STM32 pins when needed. This is to provide compatibility
  // with MobiFlight desktop app.
  MFOutput(ARDUINO_PIN pin = 0, std::string name = "LED");
  uint8_t get();
  void set(uint8_t value);

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string *buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  uint8_t _arduinoPinName;
  DigitalOut *_pin;
  uint8_t _value;
  std::string _name;
};

/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "MFModule.hpp"

class MFOutput : MFModule
{
public:
  /**
 * @brief Construct a new MFOutput::MFOutput object.
 * 
 * @param pin The Arduino pin the LED is connected to. This is also the device's ID.
 * @param name The name for this module.
 */
  MFOutput(ARDUINO_PIN pin = 0, std::string name = "LED");

  /**
   * @brief Gets the current value for the LED.
   * 
   * @return uint8_t The current value.
   */
  
  uint8_t get();
  /**
   * @brief Sets the current value for the LED.
   * 
   * @param value The current value.
   */
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

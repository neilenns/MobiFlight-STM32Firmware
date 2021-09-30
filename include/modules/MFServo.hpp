/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include "ArduinoTypes.hpp"
#include "drivers/Servo.h"
#include "MFModule.hpp"

class MFServo : MFModule
{
public:
  /**
 * @brief Construct a new MFOutput::MFOutput object.
 * 
 * @param pin The Arduino pin the LED is connected to. This is also the device's ID.
 * @param name The name for this module.
 */
  MFServo(ARDUINO_PIN pin = 0, std::string name = "LED");

  /**
   * @brief Gets the current value for the servo.
   * 
   * @return uint8_t The current value.
   */

  uint8_t get();
  /**
   * @brief Sets the current position for the servo.
   * 
   * @param value The current position. Must be between 0 and 255.
   */
  void set(uint8_t value);

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string &buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  uint8_t _arduinoPinName;
  std::shared_ptr<Servo> _servo;
  std::string _name;
  uint8_t _value;
};

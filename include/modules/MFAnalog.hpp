/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "MFModule.hpp"

class MFAnalog : MFModule
{
public:
  /**
 * @brief Construct a new MFAnalog::MFModule object.
 * 
 * @param pin The Arduino pin the analog input is connected to. This is also the device's ID.
 * @param sensitivity The change in analog input value required to send an update event.
 * @param name The name for this module.
 */
  MFAnalog(ARDUINO_PIN pin = 0, int sensitivity = 5, std::string name = "Analog");

  /**
   * @brief Reads the current analog value and sends an update via serial
   * if the change in value is greater than or equal to the configured sensitivity.
   * 
   */
  void ReadCurrentValue();

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string &buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  uint8_t _arduinoPinName;
  std::shared_ptr<AnalogIn> _pin;
  std::string _name;
  int _sensitivity;
  LowPowerTicker _ticker;
  int _value;
};
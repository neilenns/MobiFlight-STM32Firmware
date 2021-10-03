/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include <ULN2003.h>

#include "ArduinoTypes.hpp"
#include "MFModule.hpp"

class MFStepper : MFModule
{
public:
  /**
 * @brief Construct a new MFOutput::MFOutput object.
 * 
 * @param pin The Arduino pin the LED is connected to. This is also the device's ID.
 * @param name The name for this module.
 */
  MFStepper(ARDUINO_PIN pin1 = 0, ARDUINO_PIN pin2 = 0, ARDUINO_PIN pin3 = 0, ARDUINO_PIN pin4 = 0, std::string name = "Servo");

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string &buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  ARDUINO_PIN _arduinoPin1;
  ARDUINO_PIN _arduinoPin2;
  ARDUINO_PIN _arduinoPin3;
  ARDUINO_PIN _arduinoPin4;
  std::shared_ptr<ULN2003> _stepper;
  std::string _name;
};

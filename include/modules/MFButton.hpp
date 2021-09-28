/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include <iostream>

#include "ArduinoTypes.hpp"
#include "drivers/DebounceIn.hpp"
#include "MFModule.hpp"

class MFButton : MFModule
{
public:
  /**
 * @brief Construct a new MFButton::MFButton object.
 * 
 * @param pin The Arduino pin the button is connected to. This is also the device's ID.
 * @param name The name for this module.
 */
  MFButton(ARDUINO_PIN pin = 0, std::string name = "Button");

  /**
   * @brief Event handler called when the button is pressed.
   * 
   */
  void OnPress();

  /**
   * @brief Event handler called when the button is released.
   * 
   */
  void OnRelease();

  // Base class implementations
  uint8_t GetId() override;
  MFModuleType GetModuleType() override;
  void PowerSavingMode(bool state) override;
  void Serialize(std::string *buffer) override;
  void StartTest() override;
  void StopTest() override;

private:
  uint8_t _arduinoPinName;
  DebounceIn *_pin;
  DigitalOut *_led;
  std::string _name;
};

enum MFButtonState
{
  Pressed,
  Released,
};
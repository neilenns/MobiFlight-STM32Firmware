/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <bitset>
#include <mbed.h>
#include <optional>

#include "ArduinoTypes.hpp"
#include "Board.hpp"

class PinManager
{
public:
  /**
   * @brief Construct a new Pin Manager object
   * 
   */
  PinManager();

  /**
   * @brief Clears all registered pins from the list.
   * 
   */
  void ClearRegisteredPins();

  /**
   * @brief Checks to see if a pin is registered with the system.
   * 
   * @param pin The Arduino pin number to check.
   * @return true If the pin is registered.
   * @return false If the pin is not registered.
   */
  bool IsPinRegistered(ARDUINO_PIN pin);

  /**
   * @brief Registers a new pin with the specified module type.
   * 
   * @param pin The Arduino pin to register.
   */
  void RegisterPin(ARDUINO_PIN pin);

  /**
   * @brief Maps an Arduino pin number to an STM32 pin.
   * 
   * @param arduinoPin The Arduino pin number to map.
   * @return std::optional<PinName> The STM32 pin, if mapped. If the pin isn't mapped returns std::nullopt.
   */
  static std::optional<PinName> MapArudinoPin(ARDUINO_PIN arduinoPin);

private:
  // MAX_PIN_NUMBER is the highest number for a pin on the device, not the count
  // of number of pins. Add +1 when creating the bitset since it is origin zero.
  std::bitset<MAX_PIN_NUMBER + 1> _registeredPins{0};
};

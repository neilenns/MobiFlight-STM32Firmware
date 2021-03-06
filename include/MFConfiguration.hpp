/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <map>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFAnalog.hpp"
#include "modules/MFButton.hpp"
#include "modules/MFLcdDisplay.hpp"
#include "modules/MFMAX7219.hpp"
#include "modules/MFOutput.hpp"
#include "modules/MFServo.hpp"
#include "PinManager.hpp"

/**
 * @brief Maintains the configuration for all attached modules.
 * 
 */
class MFConfiguration
{
private:
  PinManager pinManager;

public:
  /**
   * @brief The name for the board.
   * 
   */
  std::string BoardName;

  /**
 * @brief List of connected analog inputs.
 * 
 */
  map<ARDUINO_PIN, std::shared_ptr<MFAnalog>> analogInputs;
  /**
 * @brief List of connected buttons.
 * 
 */
  map<ARDUINO_PIN, std::shared_ptr<MFButton>> buttons;
  /**
   * @brief List of connected LCD displays.
   * 
   */
  map<int, std::shared_ptr<MFLcdDisplay>> lcdDisplays;
  /**
   * @brief List of connected MFMAX7219 displays.
   * 
   */
  map<ARDUINO_PIN, std::shared_ptr<MFMAX7219>> ledDisplays;
  /**
   * @brief List of connected LEDs.
   * 
   */
  map<ARDUINO_PIN, std::shared_ptr<MFOutput>> outputs;
  /**
   * @brief List of connected servos.
   * 
   */
  map<ARDUINO_PIN, std::shared_ptr<MFServo>> servos;

  /**
 * @brief Construct a new MFConfiguration object
 * 
 */
  MFConfiguration();

  /**
 * @brief Adds a new analog input to the configuration.
 * 
 * @param arduinoPinName The Arduino pin the analog input is connected to. This is also the device's ID.
 * @param sensitivity The amount the input has to change before it triggers an update message.
 * @param name The name for this module.
 */
  void AddAnalogInput(ARDUINO_PIN arduinoPinName, int sensitivity, char const *name = "Button");

  /**
 * @brief Adds a new button to the configuration.
 * 
 * @param pin The Arduino pin the button is connected to. This is also the device's ID.
 * @param name The name for this module.
 */
  void AddButton(ARDUINO_PIN arduinoPinName, char const *name = "Button");

  /**
 * @brief Adds a single module from a cmdmessenger-style configuration string
 * 
 * @param moduleConfig The string containing the configuration
 */
  void AddModuleFromConfigurationString(const std::string &configuration);

  /**
 * @brief Adds multiple modules from a cmdmessenger-style configuration string
 * 
 * @param configuration The string containing the configuration
 */
  void AddModulesFromConfigurationString(const std::string &configuration);

  /**
   * @brief Adds a new LCD display to the configuration.
   * 
   * @param deviceAddress The 7-bit I2C address of the display
   * @param rows The number of rows on the display. Valid values are 1, 2, and 4.
   * @param columns The number of columns on the display. Valid values are 16 and 20.
   * @param name The name of the display.
   */
  void AddLcdDisplay(int address, int rows, int columns, char const *name = "LCD Display");

  /**
   * @brief Adds a new MAX7219 7-segment LED display to the configuration.
   * 
   * @param mosi The Arduino pin number used for MOSI. Note that in this firmware the hardware SPI_MOSI pin will always be used.
   * @param sclk The Arduino pin number used for SCLK. Note that in this firmware the hardware SPI_SCK pin will always be used.
   * @param cs The Arduino pin number used for CS. This is also the device's unique ID.
   * @param brightness The initial display brightness.
   * @param submoduleCount The number of connected submodules.
   * @param name The name for this module.
   */
  void AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int brightness, int submoduleCount, char const *name = "LED Display");

  /**
   * @brief Adds a new LED to the configuration.
   * 
   * @param pin The Arduino pin the LED is connected to. This is also the device's ID.
   * @param name The name for this module.
   */
  void AddOutput(ARDUINO_PIN arduinoPinName, char const *name = "Output");

  /**
   * @brief Adds a new servo to the configuration.
   * 
   * @param pin The Arduino pin the servo is connected to. This is also the device's ID.
   * @param name The name for this module.
   */
  void AddServo(ARDUINO_PIN arduinoPinName, char const *name = "Servo");

  /**
   * @brief Clears the list of stored modules. This does not write the updated
   * (empty) lists to flash. To do that you must call Save().
   * 
   */
  void Erase();

  /**
   * @brief Loads the list of modules stored in flash.
   * 
   */
  void Load();

  /**
   * @brief Saves the list of modules in flash.
   * 
   */
  void Save();

  /**
   * @brief Writes the entire configuration to a buffer, including board name and all modules.
   * 
   * @param buffer A string buffer to write to.
   */
  void SerializeConfiguration(std::string &buffer);

  /**
   * @brief Writes the module confugrations to a buffer.
   * 
   * @param buffer A string buffer to write to.
   */
  void SerializeModules(std::string &buffer);

  /**
   * @brief Runs the StartTest() method on all connected modules.
   * 
   */
  void StartTest();

  /**
   * @brief Runs the StopTest() method on all connected modules.
   * 
   */
  void StopTest();
};

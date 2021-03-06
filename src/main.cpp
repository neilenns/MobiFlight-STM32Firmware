/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <mbed.h>
#include <optional>

#include "ArduinoTypes.hpp"
#include "Board.hpp"
#include "fmt/core.h"
#include "Globals.hpp"
#include "MFCommands.hpp"
#include "MFConfiguration.hpp"
#include "mobiflight.hpp"
#include "PinManager.hpp"

// Board configuration
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)

constexpr inline auto type = MOBIFLIGHT_TYPE;
constexpr inline auto serial = MOBIFLIGHT_SERIAL;
constexpr inline auto version = STR_VALUE(BUILD_VERSION);

// *****************************************************************
// MobiFlight event handlers
// *****************************************************************
void OnActivateConfig()
{
  config.Load();
}

void OnConfigActivated()
{
  cmdMessenger.sendCmd(MFCommand::kConfigActivated, "OK");
}

void OnGetConfig()
{
  std::string buffer;

  config.SerializeModules(buffer);
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(buffer);
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  cmdMessenger.sendCmdStart(MFCommand::kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(config.BoardName);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(version);
  cmdMessenger.sendCmdEnd();
}

/**
 * @brief Clears all applied configurations and loads any saved configuration from flash.
 * 
 */
void OnResetBoard()
{
  config.Erase();
  config.Load();
}

void OnResetConfig()
{
  config.Erase();
}

void OnSaveConfig()
{
  config.Save();
}

void OnSetConfig()
{
  auto cfg = std::string(cmdMessenger.readStringArg());
  config.AddModulesFromConfigurationString(cfg);
  cmdMessenger.sendCmd(MFCommand::kStatus, fmt::format("{}", FLASH_USER_DATA_SIZE - cfg.length()));
}

// Displays text on the connected LCD display
void OnSetLcdText()
{
  auto address = cmdMessenger.readInt16Arg();
  auto text = cmdMessenger.readStringArg();

  auto display = config.lcdDisplays[address];
  if (!display)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Not a valid module.");
    return;
  }

  config.lcdDisplays[address]->Display(text);
}

// Displays numbers on the connected LED display
void OnSetModule()
{
  // command, module, submodule, value, points, mask;
  // 1,7,1,22222222,64,255;
  auto module = cmdMessenger.readInt16Arg();
  auto subModule = cmdMessenger.readInt16Arg();
  auto value = cmdMessenger.readStringArg();
  auto points = (uint8_t)cmdMessenger.readInt16Arg();
  auto mask = (uint8_t)cmdMessenger.readInt16Arg();

  auto display = config.ledDisplays[module];
  if (!display)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Not a valid module");
    return;
  }

  display->Display(subModule, value, points, mask);
}

void OnSetName()
{
  auto name = std::string(cmdMessenger.readStringArg());

  config.BoardName = name;

  cmdMessenger.sendCmdStart(MFCommand::kStatus);
  cmdMessenger.sendCmdArg(config.BoardName);
  cmdMessenger.sendCmdEnd();
}

// Callback function that sets led on or off
void OnSetPin()
{
  auto arduinoPin = cmdMessenger.readInt16Arg();
  auto state = cmdMessenger.readInt16Arg();

  auto output = config.outputs[arduinoPin];
  if (!output)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Not a valid module.");
    return;
  }

  output->set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(MFCommand::kStatus, std::to_string(output->get()).c_str());
}

// Callback function that sets led on or off
void OnSetServo()
{
  auto arduinoPin = cmdMessenger.readInt16Arg();
  auto state = cmdMessenger.readInt16Arg();

  auto module = config.servos[arduinoPin];
  if (!module)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "Not a valid module");
    return;
  }
  module->set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(MFCommand::kStatus, std::to_string(module->get()).c_str());
}

// Starts/stops a test of all attached output displays
void OnTest()
{
  auto state = cmdMessenger.readBoolArg();

  if (state)
  {
    config.StartTest();
    cmdMessenger.sendCmd(MFCommand::kStatus, "Test started");
  }
  else
  {
    config.StopTest();
    cmdMessenger.sendCmd(MFCommand::kStatus, "Test stopped");
  }
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(MFCommand::kStatus, "Command without attached callback");
}

// *****************************************************************
// Main methods
// *****************************************************************

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(MFCommand::kActivateConfig, OnActivateConfig);
  cmdMessenger.attach(MFCommand::kConfigActivated, OnConfigActivated);
  cmdMessenger.attach(MFCommand::kGetConfig, OnGetConfig);
  cmdMessenger.attach(MFCommand::kGetInfo, OnGetInfo);
  cmdMessenger.attach(MFCommand::kResetBoard, OnResetBoard);
  cmdMessenger.attach(MFCommand::kResetConfig, OnResetConfig);
  cmdMessenger.attach(MFCommand::kSaveConfig, OnSaveConfig);
  cmdMessenger.attach(MFCommand::kSetConfig, OnSetConfig);
  cmdMessenger.attach(MFCommand::kSetLcdDisplayI2C, OnSetLcdText);
  cmdMessenger.attach(MFCommand::kSetModule, OnSetModule);
  cmdMessenger.attach(MFCommand::kSetName, OnSetName);
  cmdMessenger.attach(MFCommand::kSetPin, OnSetPin);
  cmdMessenger.attach(MFCommand::kSetServo, OnSetServo);
  cmdMessenger.attach(MFCommand::kTest, OnTest);
  cmdMessenger.attach(OnUnknownCommand);
}

int main()
{
  queue = std::shared_ptr<EventQueue>(mbed_event_queue());

  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach all the callbacks for command messenger
  attachCommandCallbacks();

  // Reset everything and load the configuration
  OnResetBoard();

  // Temporarily add outputs
  // config.AddAnalogInput(54, 5, "Analog input");
  // config.AddOutput(4, "Onboard LED (PWM)");
  // config.AddButton(12, "Onboard button");
  // config.AddServo(6, "Servo test");
  // config.AddLedDisplay(7, 5, 10, 1, 2, "LED display 1");
  // config.AddLcdDisplay(0x27, 4, 20, "LCD display 1");

  cmdMessenger.sendCmd(MFCommand::kStatus, "STM32 has started!");

  serial_port.sigio(queue->event(callback(&cmdMessenger, &CmdMessenger::feedinSerialData)));
  queue->dispatch_forever();
}
/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <mbed.h>
#include <optional>

#include "ArduinoTypes.hpp"
#include "boards/STM32L476.h"
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
constexpr inline auto name = MOBIFLIGHT_NAME;
constexpr inline auto version = STR_VALUE(BUILD_VERSION);

// *****************************************************************
// MobiFlight event handlers
// *****************************************************************
void OnConfigActivated()
{
  cmdMessenger.sendCmd(MFCommand::kConfigActivated, "OK");
}

void OnGetConfig()
{
  std::string buffer;

  config.Serialize(buffer);
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(buffer);
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  cmdMessenger.sendCmdStart(MFCommand::kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(version);
  cmdMessenger.sendCmdEnd();
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

// Callback function that sets led on or off
void OnSetPin()
{
  auto arduinoPin = cmdMessenger.readInt16Arg();
  auto state = cmdMessenger.readInt16Arg();

  auto LED = config.outputs[arduinoPin];
  LED->set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(MFCommand::kStatus, std::to_string(LED->get()).c_str());
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
  cmdMessenger.attach(MFCommand::kConfigActivated, OnConfigActivated);
  cmdMessenger.attach(MFCommand::kGetConfig, OnGetConfig);
  cmdMessenger.attach(MFCommand::kGetInfo, OnGetInfo);
  cmdMessenger.attach(MFCommand::kSetLcdDisplayI2C, OnSetLcdText);
  cmdMessenger.attach(MFCommand::kSetModule, OnSetModule);
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

  // Temporarily add outputs
  config.AddOutput(13, "Onboard LED (PWM)");
  config.AddButton(12, "Onboard button");
  config.AddServo(6, "Servo test");
  config.AddLedDisplay(7, 5, 10, 2, "LED display 1");
  config.AddLcdDisplay(0x27, 4, 20, "LCD display 1");

  cmdMessenger.sendCmd(MFCommand::kStatus, "STM32 has started!");

  serial_port.sigio(queue->event(callback(&cmdMessenger, &CmdMessenger::feedinSerialData)));
  queue->dispatch_forever();
}
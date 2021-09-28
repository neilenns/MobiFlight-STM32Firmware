/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <mbed.h>
#include <optional>

#include "ArduinoTypes.hpp"
#include "boards/STM32L476.h"
#include "CmdMessenger.hpp"
#include "MFCommands.hpp"
#include "MFConfiguration.hpp"
#include "mobiflight.hpp"
#include "PinManager.hpp"

// Modules
#include "modules/MFModuleTypes.hpp"
#include "modules/MFOutput.hpp"

BufferedSerial serial_port(USBTX, USBRX, 115200);
Thread t;

// Globals
CmdMessenger cmdMessenger = CmdMessenger(serial_port);
MFConfiguration config;

// Board configuration
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

const uint8_t MEM_OFFSET_NAME = 0;
const uint8_t MEM_LEN_NAME = 48;
const uint8_t MEM_OFFSET_SERIAL = MEM_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t MEM_LEN_SERIAL = 11;
const uint8_t MEM_OFFSET_CONFIG = MEM_OFFSET_NAME + MEM_LEN_NAME + MEM_LEN_SERIAL;

char type[20] = MOBIFLIGHT_TYPE;
char serial[MEM_LEN_SERIAL] = MOBIFLIGHT_SERIAL;
char name[MEM_LEN_NAME] = MEMLEN_NAME;
const int MEM_LEN_CONFIG = MEMLEN_CONFIG;

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

  config.Serialize(&buffer);
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
  cmdMessenger.sendCmdArg(VERSION);
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
  auto state = cmdMessenger.readBoolArg();

  auto LED = config.outputs[arduinoPin];
  LED->set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(MFCommand::kStatus, std::to_string(LED->get()).c_str());
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
  cmdMessenger.attach(MFCommand::kTest, OnTest);
  cmdMessenger.attach(OnUnknownCommand);
}

int main()
{
  auto *queue = mbed_event_queue();
  t.start(callback(queue, &EventQueue::dispatch_forever));

  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach all the callbacks for command messenger
  attachCommandCallbacks();

  // Temporarily add outputs
  config.AddOutput(4, "Onboard LED1"); // Should NOT be PWM
  // config.AddButton(3, "Onboard button");
  config.AddOutput(11, "Onboard LED2"); // Should be PWM
  // config.AddLedDisplay(7, 5, 10, 2, "LED display 1");
  // config.AddLcdDisplay(0x27, 4, 20, "LCD display 1");

  cmdMessenger.sendCmd(MFCommand::kStatus, "STM32 has started!");

  serial_port.sigio(queue->event(callback(&cmdMessenger, &CmdMessenger::feedinSerialData)));
}
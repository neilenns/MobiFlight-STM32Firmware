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
  cmdMessenger.sendCmd(kConfigActivated, "OK");
}

void OnGetConfig()
{
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(config);
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(VERSION);
  cmdMessenger.sendCmdEnd();
}

// Displays text on the connected module
void OnSetModule()
{
  // command, module, submodule, value, points, mask;
  // 1,7,0,87654321,0,255;
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  char *value = cmdMessenger.readStringArg();
  uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
  uint8_t mask = (uint8_t)cmdMessenger.readInt16Arg();

  auto display = config.ledDisplays[module];
  if (!display)
  {
    cmdMessenger.sendCmd(kStatus, "Not a valid module");
    return;
  }

  display->Display(subModule, value, points, mask);
}

// Callback function that sets led on or off
void OnSetPin()
{
  int arduinoPin = cmdMessenger.readInt16Arg();
  int state = cmdMessenger.readBoolArg();

  auto LED = config.outputs[arduinoPin];
  LED->set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(kStatus, std::to_string(LED->get()).c_str());
}

// Starts/stops a test of all attached output displays
void OnTest()
{
  int state = cmdMessenger.readBoolArg();

  if (state)
  {
    config.StartTest();
    cmdMessenger.sendCmd(kStatus, "Test started");
  }
  else
  {
    config.StopTest();
    cmdMessenger.sendCmd(kStatus, "Test stopped");
  }
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kStatus, "Command without attached callback");
}

// *****************************************************************
// Main methods
// *****************************************************************

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kConfigActivated, OnConfigActivated);
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kSetPin, OnSetPin);
  cmdMessenger.attach(kSetModule, OnSetModule);
  cmdMessenger.attach(kTest, OnTest);
}

int main()
{
  EventQueue *queue = mbed_event_queue();
  t.start(callback(queue, &EventQueue::dispatch_forever));

  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach all the callbacks for command messenger
  attachCommandCallbacks();

  // Temporarily add two outputs
  config.AddOutput(2, "Onboard LED1");
  config.AddButton(3, "Onboard button");
  config.AddOutput(4, "Onboard LED2");
  config.AddLedDisplay(7, 5, 10, "LED display 1");

  cmdMessenger.sendCmd(kStatus, "STM32 has started!");

  serial_port.sigio(queue->event(callback(&cmdMessenger, &CmdMessenger::feedinSerialData)));
}
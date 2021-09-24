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

// Command messenger configuration
CmdMessenger cmdMessenger = CmdMessenger(serial_port);
unsigned long lastCommand;

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

char configBuffer[MEM_LEN_CONFIG] = "";

// Pins and configuration
PinManager pinManager;

MFConfiguration config;

// *****************************************************************
// Module management
// *****************************************************************
void AddButton(ARDUINO_PIN arduinoPinName, char const *name = "Button")
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, "Duplicate pin.");
#endif
    return;
  }

  config.buttons.insert({arduinoPinName, new MFButton(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName, MFModuleType::kButton);
}

void AddOutput(ARDUINO_PIN arduinoPinName, char const *name = "Output")
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, "Duplicate pin.");
#endif
    return;
  }

  config.outputs.insert({arduinoPinName, new MFOutput(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName, MFModuleType::kOutput);
}

// *****************************************************************
// MobiFlight event handlers
// *****************************************************************
void OnConfigActivated()
{
  cmdMessenger.sendCmd(kConfigActivated, "OK");
}

void OnGetConfig()
{
  lastCommand = millis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(config);
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  lastCommand = millis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(VERSION);
  cmdMessenger.sendCmdEnd();
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
}

int main()
{
  t.start(callback(mbed_event_queue(), &EventQueue::dispatch_forever));

  pinManager.ClearRegisteredPins();

  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach all the callbacks for command messenger
  attachCommandCallbacks();

  // Temporarily add two outputs
  AddOutput(2, "Onboard LED1");
  AddButton(3, "Onboard button");
  AddOutput(4, "Onboard LED2");

  cmdMessenger.sendCmd(kStatus, "STM32 has started!");

  while (1)
  {
    cmdMessenger.feedinSerialData();
  }
}
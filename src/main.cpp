#include <mbed.h>
#include <optional>

#include "CmdMessenger.hpp"
#include "MFCommands.hpp"
#include "mobiflight.hpp"
#include "boards/STM32L476.h"
#include "PinManager.hpp"

// Modules
#include "modules/MFModuleTypes.hpp"
#include "modules/MFOutput.hpp"

static BufferedSerial serial_port(USBTX, USBRX, 115200);
static DigitalOut led(LED1);

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

// Modules
PinManager pinManager;
map<PinName, MFOutput> outputs;

FileHandle *mbed::mbed_override_console(int fd)
{
  return &serial_port;
}

// *****************************************************************
// Module management
// *****************************************************************
void AddOutput(PinName pin, char const *name = "Output")
{
  if (pinManager.IsPinRegistered(pin))
  {
    return;
  }

  outputs[pin] = MFOutput(pin);
  pinManager.RegisterPin(pin, MFModuleType::kOutput);
}

// *****************************************************************
// MobiFlight event handlers
// *****************************************************************

void OnGetConfig()
{
  lastCommand = millis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(configBuffer);
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
  std::optional<PinName> stm32pin = pinManager.MapArudinoPin(arduinoPin);

  if (!stm32pin)
  {
    cmdMessenger.sendCmd(kStatus, "The requested pin is not supported on this board.");
    return;
  }

  outputs[*stm32pin].set(state);

  // Send back status that describes the led state
  cmdMessenger.sendCmd(kStatus, std::to_string(outputs[*stm32pin].get()));
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
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kSetPin, OnSetPin);
}

int main()
{
  pinManager.ClearRegisteredPins();

  // Adds newline to every command
  cmdMessenger.printLfCr();

  // Attach my application's user-defined callback methods
  attachCommandCallbacks();

  // Temporarily add an output
  AddOutput(LED1, "Onboard LED");

  // Send the status to the PC that says the Arduino has booted
  // Note that this is a good debug function: it will let you also know
  // if your program had a bug and the arduino restarted
  cmdMessenger.sendCmd(kStatus, "STM32 has started!");

  while (1)
  {
    cmdMessenger.feedinSerialData();
    ThisThread::sleep_for(500ms);
  }
}
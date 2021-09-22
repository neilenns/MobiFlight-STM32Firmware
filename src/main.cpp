#include <mbed.h>

#include "CmdMessenger.h"
#include "MFCommands.h"
#include "mobiflight.h"
#include "boards/STM32L476.h"

static BufferedSerial serial_port(USBTX, USBRX, 115200);
static DigitalOut led(LED1);
bool ledState = 0; // Current state of Led

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

FileHandle *mbed::mbed_override_console(int fd)
{
  return &serial_port;
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kStatus, "Command without attached callback");
}

// Callback function that sets led on or off
void OnSetPin()
{
  // Read led state argument, interpret string as boolean
  ledState = cmdMessenger.readBoolArg();
  // Set led
  led = ledState ? 1 : 0;

  // Send back status that describes the led state
  cmdMessenger.sendCmd(kStatus, (int)ledState);
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

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kSetPin, OnSetPin);
}

int main()
{
  // Adds newline to every command
  cmdMessenger.printLfCr();
  // Attach my application's user-defined callback methods
  attachCommandCallbacks();
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
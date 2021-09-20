#include <mbed.h>
#include <CmdMessenger.h>

#define MAXIMUM_BUFFER_SIZE 32

static BufferedSerial serial_port(USBTX, USBRX, 115200);
static DigitalOut led(LED1);
bool ledState = 0; // Current state of Led

// Command messenger configuration
CmdMessenger cmdMessenger = CmdMessenger(serial_port);

// This is the list of recognized commands. These can be commands that can either be sent or received.
// In order to receive, attach a callback function to these events
enum
{
  kSetLed, // Command to request led to be set in specific state
  kStatus, // Command to report status
};

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
void OnSetLed()
{
  // Read led state argument, interpret string as boolean
  ledState = cmdMessenger.readBoolArg();
  // Set led
  led = ledState ? 1 : 0;

  // Send back status that describes the led state
  cmdMessenger.sendCmd(kStatus, (int)ledState);
}

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  // Attach callback methods
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kSetLed, OnSetLed);
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
}
#include <fmt/core.h>
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "Globals.hpp"
#include "MFCommands.hpp"
#include "modules/MFButton.hpp"
#include "PinManager.hpp"

extern BufferedSerial serial_port;

MFButton::MFButton(ARDUINO_PIN arduinoPinName, std::string name)
{
  EventQueue *queue = mbed_event_queue();
  _arduinoPinName = arduinoPinName;

  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32pin = PinManager::MapArudinoPin(arduinoPinName);
  if (!stm32pin)
  {
    // This should do something smarter
    return;
  }

  _pin = new DebounceIn(*stm32pin);
  _pin->fall(queue->event(callback(this, &MFButton::OnPress)));
  _pin->rise(queue->event(callback(this, &MFButton::OnRelease)));
  _name = name;
}

// Example of what good output looks like:
// 1.14.Button:
void MFButton::Serialize(std::string *buffer)
{
  // MobiFlight expects a trailing : at the end of every serialized module.
  buffer->append(fmt::format("{}.{}.{}:", as_integer(MFModuleType::kButton), _arduinoPinName, _name));
}

void MFButton::OnPress()
{
  cmdMessenger.sendCmdStart(as_integer(MFCommand::kButtonChange));
  cmdMessenger.sendCmdArg(_name);
  cmdMessenger.sendCmdArg(1);
  cmdMessenger.sendCmdEnd();
}

void MFButton::OnRelease()
{
  cmdMessenger.sendCmdStart(as_integer(MFCommand::kButtonChange));
  cmdMessenger.sendCmdArg(_name);
  cmdMessenger.sendCmdArg(0);
  cmdMessenger.sendCmdEnd();
}
#include <mbed.h>

#include "ArduinoTypes.hpp"
#include "modules/MFButton.hpp"
#include "PinManager.hpp"

extern EventQueue queue;

MFButton::MFButton(ARDUINO_PIN arduinoPinName, std::string name)
{
  _arduinoPinName = arduinoPinName;

  // TODO: Handle the case where an invalid pin is specified
  std::optional<PinName> stm32pin = PinManager::MapArudinoPin(arduinoPinName);
  if (!stm32pin)
  {
    // This should do something smarter
    return;
  }

  _pin = new InterruptIn(*stm32pin);
  _pin->fall(queue.event(callback(this, &MFButton::OnPress)));
  _pin->rise(queue.event(callback(this, &MFButton::OnRelease)));
  _name = name;
}

// Example of what good output looks like:
// 1.14.Button:
void MFButton::Serialize(char *str, size_t len)
{
  snprintf(str, len, "%i.%i.%s", as_integer(MFModuleType::kButton), _arduinoPinName, _name.c_str());
  str[len - 1] = '\0';
}

void MFButton::OnPress()
{
}

void MFButton::OnRelease()
{
}
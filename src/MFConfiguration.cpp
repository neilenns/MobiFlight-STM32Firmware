#include <map>
#include <mbed.h>

#include "Globals.hpp"
#include "MFConfiguration.hpp"

#define MAX_BUFFER_SIZE 40
char buffer[40];

void MFConfiguration::AddButton(ARDUINO_PIN arduinoPinName, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, "Duplicate pin.");
#endif
    return;
  }

  buttons.insert({arduinoPinName, new MFButton(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName, MFModuleType::kButton);
}

void MFConfiguration::AddOutput(ARDUINO_PIN arduinoPinName, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, "Duplicate pin.");
#endif
    return;
  }

  outputs.insert({arduinoPinName, new MFOutput(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName, MFModuleType::kOutput);
}

void MFConfiguration::Load()
{
}

void MFConfiguration::Erase()
{
}

void MFConfiguration::Serialize()
{
  for (auto &[key, value] : outputs)
  {
    value->Serialize(buffer, sizeof(buffer));
    printf(buffer);
    printf(":");
  }

  for (auto &[key, value] : buttons)
  {
    value->Serialize(buffer, sizeof(buffer));
    printf(buffer);
    printf(":");
  }
}
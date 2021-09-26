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

void MFConfiguration::AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount, char const *name)
{
  if (pinManager.IsPinRegistered(mosi) || pinManager.IsPinRegistered(sclk) || pinManager.IsPinRegistered(cs))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, "Duplicate pin.");
#endif
    return;
  }

  // MOSI a.k.a. the data pin is used as the identifier for the connected module
  // by MobiFlight.
  ledDisplays.insert({mosi, new MFMAX7219(mosi, sclk, cs, submoduleCount, name)});
  pinManager.RegisterPin(mosi, MFModuleType::kLedSegment);
  pinManager.RegisterPin(sclk, MFModuleType::kLedSegment);
  pinManager.RegisterPin(cs, MFModuleType::kLedSegment);
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

  for (auto &[key, value] : ledDisplays)
  {
    value->Serialize(buffer, sizeof(buffer));
    printf(buffer);
    printf(":");
  }
}

void MFConfiguration::StartTest()
{
  for (auto &[key, value] : ledDisplays)
  {
    value->StartTest();
  }
}

void MFConfiguration::StopTest()
{
  for (auto &[key, value] : ledDisplays)
  {
    value->StopTest();
  }
}
#include <map>
#include <mbed.h>
#include <TextLCD.h>

#include "Globals.hpp"
#include "MFCommands.hpp"
#include "MFConfiguration.hpp"

void MFConfiguration::AddButton(ARDUINO_PIN arduinoPinName, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
#endif
    return;
  }

  buttons.insert({arduinoPinName, new MFButton(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName, MFModuleType::kButton);
}

void MFConfiguration::AddLcdDisplay(int address, int rows, int columns, char const *name)
{
  if (lcdDisplays.count(address))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate LCD display address.");
#endif
    return;
  }

  lcdDisplays.insert({address, new MFLcdDisplay(address, rows, columns, name)});
}

void MFConfiguration::AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int submoduleCount, char const *name)
{
  if (pinManager.IsPinRegistered(mosi) || pinManager.IsPinRegistered(sclk) || pinManager.IsPinRegistered(cs))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
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
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
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
  std::string buffer;

  for (auto &[key, output] : outputs)
  {
    output->Serialize(&buffer);
  }

  for (auto &[key, button] : buttons)
  {
    button->Serialize(&buffer);
  }

  for (auto &[key, ledDisplay] : ledDisplays)
  {
    ledDisplay->Serialize(&buffer);
  }

  for (auto &[key, lcdDisplay] : lcdDisplays)
  {
    lcdDisplay->Serialize(&buffer);
  }

  printf(buffer.c_str());
}

void MFConfiguration::StartTest()
{
  for (auto &[key, value] : lcdDisplays)
  {
    value->StartTest();
  }
}

void MFConfiguration::StopTest()
{
  for (auto &[key, value] : lcdDisplays)
  {
    value->StopTest();
  }
}
#include <FlashIAP.h>
#include <map>
#include <mbed.h>

#include "Globals.hpp"
#include "MFCommands.hpp"
#include "MFConfiguration.hpp"

#define FLASH_USER_DATA_START 0x080E0000

#define MAX_BUFFER_SIZE 2048
char buffer[MAX_BUFFER_SIZE];

static char userConfig[64] __attribute__((__section__(".user_data")));

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

void MFConfiguration::Erase()
{
}

void MFConfiguration::Load()
{
  cmdMessenger.sendCmd(kStatus, userConfig);
}

void MFConfiguration::Save()
{
  auto flash = new FlashIAP();

  buffer[0] = 'H';
  buffer[1] = 'e';
  buffer[2] = 'l';
  buffer[3] = 'l';
  buffer[4] = 'o';
  buffer[5] = '\0';

  flash->init();
  // volatile auto sectorSize = flash->get_sector_size(FLASH_USER_DATA_START);
  auto status = flash->erase(FLASH_USER_DATA_START, MAX_BUFFER_SIZE);
  flash->program(buffer, FLASH_USER_DATA_START, MAX_BUFFER_SIZE);
  flash->deinit();
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
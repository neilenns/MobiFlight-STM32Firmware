#include <FlashIAP.h>
#include <map>
#include <mbed.h>

#include "Globals.hpp"
#include "MFConfiguration.hpp"

#define FLASH_USER_DATA_START 0x080E0000
#define FLASH_USER_DATA_SIZE 0x20000 // 128k

#define MAX_BUFFER_SIZE 40
char buffer[40];

static uint8_t userConfig[64] __attribute__((__section__(".user_data")));
static const char *testBuffer = "Hello";

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
  auto flash = new FlashIAP();

  flash->init();
  printf("%lu", flash->get_flash_size());
  flash->erase(FLASH_USER_DATA_START, FLASH_USER_DATA_SIZE);
  flash->program(testBuffer, FLASH_USER_DATA_START, sizeof(testBuffer));
  printf("%u", (unsigned int)userConfig);
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
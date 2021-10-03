/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <FlashIAP.h>
#include <fmt/core.h>
#include <map>
#include <mbed.h>
#include <TextLCD.h>

#include "Globals.hpp"
#include "MFCommands.hpp"
#include "MFConfiguration.hpp"

#define FLASH_USER_DATA_START 0x080FF800
#define FLASH_USER_DATA_SIZE 2048

static char userConfig[FLASH_USER_DATA_SIZE] __attribute__((__section__(".user_data")));

void MFConfiguration::AddButton(ARDUINO_PIN arduinoPinName, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
#endif
    return;
  }

  buttons.insert({arduinoPinName, std::make_shared<MFButton>(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName);
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

  lcdDisplays.insert({address, std::make_shared<MFLcdDisplay>(address, rows, columns, name)});
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
  ledDisplays.insert({mosi, std::make_shared<MFMAX7219>(mosi, sclk, cs, submoduleCount, name)});
  pinManager.RegisterPin(mosi);
  pinManager.RegisterPin(sclk);
  pinManager.RegisterPin(cs);
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

  outputs.insert({arduinoPinName, std::make_shared<MFOutput>(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName);
}

void MFConfiguration::AddServo(ARDUINO_PIN arduinoPinName, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
#endif
    return;
  }

  servos.insert({arduinoPinName, std::make_shared<MFServo>(arduinoPinName, name)});
  pinManager.RegisterPin(arduinoPinName);
}

void MFConfiguration::AddStepper(ARDUINO_PIN arduinoPin1, ARDUINO_PIN arduinoPin2, ARDUINO_PIN arduinoPin3, ARDUINO_PIN arduinoPin4, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPin1) ||
      pinManager.IsPinRegistered(arduinoPin2) ||
      pinManager.IsPinRegistered(arduinoPin3) ||
      pinManager.IsPinRegistered(arduinoPin4))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
#endif
    return;
  }

  steppers.insert({arduinoPin1, std::make_shared<MFStepper>(arduinoPin1, arduinoPin2, arduinoPin3, arduinoPin4, name)});
  pinManager.RegisterPin(arduinoPin1);
  pinManager.RegisterPin(arduinoPin2);
  pinManager.RegisterPin(arduinoPin3);
  pinManager.RegisterPin(arduinoPin4);
}

void MFConfiguration::Load()
{
  // The first test of a valid configuration is to check for MF; stored
  // at the appropriate location in flash. If it's not there
  if (strncmp(userConfig, flashIdentifier.data(), 2) != 0)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "No configuration saved in flash.");
    return;
  }

  const std::string_view storedConfiguration(userConfig);
  cmdMessenger.sendCmd(MFCommand::kGetConfig, userConfig);
}

void MFConfiguration::Erase()
{
}

void MFConfiguration::Save()
{
  int result;
  auto flash = new FlashIAP();
  std::string buffer;

  // The configuration is stored in flash in the same format as the commands sent from
  // MobiFlight to add the modules in the first place. To make it easy to know if a
  // config is stored in flash it gets prepended with "MF:" and a flash format
  // version number just in case something has to change in the future and
  // support for upgrading from a prior stored format is required.
  buffer.append(fmt::format("{}:{}:", flashIdentifier, flashStorageVersion));
  Serialize(buffer);

  result = flash->init();
  if (result != 0)
  {
    cmdMessenger.sendCmd(kStatus, fmt::format("Error initializing flash: {0}", result));
    return;
  }
  // Pad the string out to a multiple of the sector size. STM32 devices
  // require that flash be written an entire sector at a time.
  auto sectorSize = flash->get_sector_size(FLASH_USER_DATA_START);
  if (sectorSize == MBED_FLASH_INVALID_SIZE)
  {
    cmdMessenger.sendCmd(kStatus, "Error getting flash sector size.");
    return;
  }
  auto padAmount = sectorSize - (buffer.length() % sectorSize);
  buffer.append(padAmount, '\0');

  // Make sure the total size of the configuration isn't bigger than the available flash space.
  // This seems incredibly unlikely to happen but better safe than sorry.
  if (buffer.length() > FLASH_USER_DATA_SIZE)
  {
    cmdMessenger.sendCmd(kStatus, fmt::format("The configuration length ({0}) is too long to store in the available flash space.", buffer.length()));
    return;
  }

  // The flash must be erased before writing data.
  result = flash->erase(FLASH_USER_DATA_START, FLASH_USER_DATA_SIZE);
  if (result != 0)
  {
    cmdMessenger.sendCmd(kStatus, fmt::format("Error erasing flash: {0}", result));
    return;
  }

  // Write the actual data.
  result = flash->program(buffer.c_str(), FLASH_USER_DATA_START, buffer.length());
  if (result != 0)
  {
    cmdMessenger.sendCmd(kStatus, fmt::format("Error programming flash: {0}", result));
    return;
  }

  result = flash->deinit();
  if (result != 0)
  {
    cmdMessenger.sendCmd(kStatus, fmt::format("Error closing flash: {0}", result));
    return;
  }

  cmdMessenger.sendCmd(kStatus, "Configuration saved");
}

void MFConfiguration::Serialize(std::string &buffer)
{
  for (auto &[key, output] : outputs)
  {
    output->Serialize(buffer);
  }

  for (auto &[key, button] : buttons)
  {
    button->Serialize(buffer);
  }

  for (auto &[key, ledDisplay] : ledDisplays)
  {
    ledDisplay->Serialize(buffer);
  }

  for (auto &[key, lcdDisplay] : lcdDisplays)
  {
    lcdDisplay->Serialize(buffer);
  }

  for (auto &[key, servo] : servos)
  {
    servo->Serialize(buffer);
  }

  for (auto &[key, stepper] : steppers)
  {
    stepper->Serialize(buffer);
  }
}

void MFConfiguration::StartTest()
{
  for (auto &[key, value] : steppers)
  {
    value->StartTest();
  }

  // for (auto &[key, value] : ledDisplays)
  // {
  //   value->StartTest();
  // }

  // for (auto &[key, value] : lcdDisplays)
  // {
  //   value->StartTest();
  // }
  // for (auto &[key, value] : outputs)
  // {
  //   value->StartTest();
  // }
}

void MFConfiguration::StopTest()
{
  for (auto &[key, value] : lcdDisplays)
  {
    value->StopTest();
  }

  for (auto &[key, value] : ledDisplays)
  {
    value->StopTest();
  }

  for (auto &[key, value] : outputs)
  {
    value->StopTest();
  }
}
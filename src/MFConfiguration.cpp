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
#include "stringutils.hpp"

#define FLASH_USER_DATA_START 0x080FF800
#define FLASH_USER_DATA_SIZE 2048

static char userConfig[FLASH_USER_DATA_SIZE] __attribute__((__section__(".user_data")));

void MFConfiguration::AddAnalogInput(ARDUINO_PIN arduinoPinName, int sensitivity, char const *name)
{
  if (pinManager.IsPinRegistered(arduinoPinName))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(MFCommand::kStatus, "Duplicate pin.");
#endif
    return;
  }

  analogInputs.insert({arduinoPinName, std::make_shared<MFAnalog>(arduinoPinName, sensitivity, name)});
  pinManager.RegisterPin(arduinoPinName);
}

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

void MFConfiguration::AddFromConfigurationString(const std::string &configuration)
{
  std::vector<std::string> modules;

  // The configuration string consists of multiple module configs separated by a colon
  stringutils::split(configuration, ":", modules);

  for (auto module : modules)
  {
    std::vector<std::string> moduleConfig;

    // The module configuration is split by periods
    stringutils::split(module, ".", moduleConfig);

    // There is always an empty module config at the end of the configuration string
    if (moduleConfig.size() == 1)
    {
      continue;
    }

    // There should always be at least three parts to a module configuration
    if (moduleConfig.size() < 3)
    {
      cmdMessenger.sendCmd(MFCommand::kStatus, "Invalid module configuration received");
      continue;
    }

    // Sample configuration string
    // 11.54.5.Analog input:1.12.Onboard button:4.7.5.10.1.2.LED display 1:7.39.20.4.LCD display 1:3.4.Onboard LED (PWM):6.6.Servo test:;
    // Figure out what kind of module it is then add it
    switch (static_cast<MFModuleType>(atoi(moduleConfig[0].c_str())))
    {
    case MFModuleType::kButton:
    {
      auto pin = atoi(moduleConfig[1].c_str());
      auto name = moduleConfig[2].c_str();

      AddButton(pin, name);
      break;
    }

    case MFModuleType::kOutput:
    {
      auto pin = atoi(moduleConfig[1].c_str());
      auto name = moduleConfig[2].c_str();

      AddOutput(pin, name);
      break;
    }

    case MFModuleType::kLedSegment:
    {

      auto data = atoi(moduleConfig[1].c_str());
      auto cs = atoi(moduleConfig[2].c_str());
      auto clk = atoi(moduleConfig[3].c_str());
      auto brightness = atoi(moduleConfig[4].c_str());
      auto submodules = atoi(moduleConfig[5].c_str());
      auto name = moduleConfig[6].c_str();

      AddLedDisplay(data, clk, cs, brightness, submodules, name);
      break;
    }

    case MFModuleType::kServo:
    {
      auto pin = atoi(moduleConfig[1].c_str());
      auto name = moduleConfig[2].c_str();

      AddServo(pin, name);
      break;
    }

    case MFModuleType::kLcdDisplayI2C:
    {

      auto address = atoi(moduleConfig[1].c_str());
      auto columns = atoi(moduleConfig[2].c_str());
      auto rows = atoi(moduleConfig[3].c_str());
      auto name = moduleConfig[4].c_str();

      AddLcdDisplay(address, rows, columns, name);
      break;
    }

    case MFModuleType::kAnalogInput:
    {
      auto pin = atoi(moduleConfig[1].c_str());
      auto sensitivity = atoi(moduleConfig[2].c_str());
      auto name = moduleConfig[3].c_str();

      AddAnalogInput(pin, sensitivity, name);
      break;
    }

    default:
    {
      cmdMessenger.sendCmd(MFCommand::kStatus, fmt::format("Unsupported module type {}", moduleConfig[0]));
      break;
    }
    }
  }
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

void MFConfiguration::AddLedDisplay(ARDUINO_PIN mosi, ARDUINO_PIN sclk, ARDUINO_PIN cs, int brightness, int submoduleCount, char const *name)
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
  ledDisplays.insert({mosi, std::make_shared<MFMAX7219>(mosi, sclk, cs, brightness, submoduleCount, name)});
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

void MFConfiguration::Load()
{
  // The first test of a valid configuration is to check for MF; stored
  // at the appropriate location in flash. If it's not there
  if (strncmp(userConfig, flashIdentifier.data(), 2) != 0)
  {
    cmdMessenger.sendCmd(MFCommand::kStatus, "No configuration saved in flash.");
    return;
  }
}

void MFConfiguration::Erase()
{
  analogInputs.clear();
  buttons.clear();
  ledDisplays.clear();
  lcdDisplays.clear();
  outputs.clear();
  servos.clear();
  pinManager.ClearRegisteredPins();

  cmdMessenger.sendCmd(kStatus, "OK");
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

  cmdMessenger.sendCmd(kConfigSaved, "OK");
}

void MFConfiguration::Serialize(std::string &buffer)
{
  for (auto &[key, analogInput] : analogInputs)
  {
    analogInput->Serialize(buffer);
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

  for (auto &[key, output] : outputs)
  {
    output->Serialize(buffer);
  }

  for (auto &[key, servo] : servos)
  {
    servo->Serialize(buffer);
  }
}

void MFConfiguration::StartTest()
{
  // for (auto &[key, value] : servos)
  // {
  //   value->StartTest();
  // }

  for (auto &[key, value] : ledDisplays)
  {
    value->StartTest();
  }

  for (auto &[key, value] : lcdDisplays)
  {
    value->StartTest();
  }
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
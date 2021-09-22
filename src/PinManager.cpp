#include <map>
#include <optional>
#include <mbed.h>

#include "boards/STM32L476.h"
#include "modules/MFModuleTypes.hpp"
#include "PinManager.hpp"

PinManager::PinManager(uint8_t numberOfPins)
{
  _registeredPins = new map<uint8_t, MFModuleType>;
}

// Sets all pins to kNotSet
void PinManager::ClearRegisteredPins()
{
  _registeredPins->clear();
}

// Returns true if the specified pin is registered
bool PinManager::IsPinRegistered(ARDUINO_PIN arduinoPinName)
{
  return _registeredPins->count(arduinoPinName) > 0;
}

// Registers a pin as the specified type
void PinManager::RegisterPin(ARDUINO_PIN arduinoPinName, MFModuleType type)
{
  (*_registeredPins)[arduinoPinName] = type;
}

// Returns the equivalent STM32 pin for a given Arduino pin
std::optional<PinName> PinManager::MapArudinoPin(ARDUINO_PIN arduinoPin)
{
  std::map<int, PinName>::const_iterator stm32pin = PinMappings.find(arduinoPin);

  if (stm32pin == PinMappings.end())
  {
    return std::nullopt;
  }

  return stm32pin->second;
}
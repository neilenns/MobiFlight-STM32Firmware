#include <map>
#include <optional>
#include <mbed.h>

#include "boards/STM32L476.h"
#include "modules/MFModuleTypes.hpp"
#include "PinManager.hpp"

PinManager::PinManager(uint8_t numberOfPins)
{
  _registeredPins = new map<PinName, MFModuleType>;
}

// Sets all pins to kNotSet
void PinManager::ClearRegisteredPins()
{
  _registeredPins->clear();
}

// Returns true if the specified pin is registered
bool PinManager::IsPinRegistered(PinName pin)
{
  return _registeredPins->count(pin) > 0;
}

// Registers a pin as the specified type
void PinManager::RegisterPin(PinName pin, MFModuleType type)
{
  (*_registeredPins)[pin] = type;
}

// Returns the equivalent STM32 pin for a given Arduino pin
std::optional<PinName> PinManager::MapArudinoPin(int arduinoPin)
{
  std::map<int, PinName>::const_iterator stm32pin = PinMappings.find(arduinoPin);

  if (stm32pin == PinMappings.end())
  {
    return std::nullopt;
  }

  return stm32pin->second;
}
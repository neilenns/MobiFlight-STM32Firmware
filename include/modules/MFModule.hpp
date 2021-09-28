/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include "MFModuleTypes.hpp"

class MFModule
{
public:
  /**
   * @brief Get the identifier for the object.
   * 
   * @return int The object identifier.
   */
  virtual uint8_t GetId() = 0;

  /**
   * @brief Get the type of the module.
   * 
   * @return MFModuleType The module type.
   */
  virtual MFModuleType GetModuleType() = 0;

  /**
 * @brief Enables or disables power saving mode.
 * 
 * @param state True to enable power saving mode, false to disable it.
 */
  virtual void PowerSavingMode(bool state) = 0;

  /**
 * @brief Writes the configuraton to a string.
 * 
 * @param buffer Pointer to the string buffer to write the configuration to.
 */
  virtual void Serialize(std::string *buffer) = 0;

  /**
 * @brief Turns on test mode for the module.
 */
  virtual void StartTest() = 0;

  /**
 * @brief Turns off test mode for the module.
 */
  virtual void StopTest() = 0;
};
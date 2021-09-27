/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <iostream>

#include "MFModuleTypes.hpp"

class MFModule
{
public:
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
 * @param str Buffer to write the configuration to.
 * @param len Length of the buffer.
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
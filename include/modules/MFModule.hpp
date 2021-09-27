/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <iostream>

class MFModule
{
public:
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
};
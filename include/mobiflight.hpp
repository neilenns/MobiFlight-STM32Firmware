/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <mbed.h>
#include "modules/MFModuleTypes.hpp"

// Event handlers
void OnGetInfo();   // kGetInfo
void OnGetConfig(); // kGetConfig
void OnSetConfig(); // kSetConfig
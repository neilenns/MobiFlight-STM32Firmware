#pragma once

#include <mbed.h>
#include "modules/MFModuleTypes.hpp"

// Event handlers
void OnGetInfo();   // kGetInfo
void OnGetConfig(); // kGetConfig
void OnSetConfig(); // kSetConfig
/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include "CmdMessenger.hpp"
#include "MFConfiguration.hpp"

inline BufferedSerial serial_port(USBTX, USBRX, 115200);
inline CmdMessenger cmdMessenger = CmdMessenger(serial_port);

inline std::shared_ptr<EventQueue> queue;
inline MFConfiguration config;
inline constexpr auto flashIdentifier = "MF"sv;
inline constexpr auto flashStorageVersion = "1.0.0"sv;

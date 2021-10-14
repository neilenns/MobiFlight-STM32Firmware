/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#include <map>
#include <mbed.h>

#define MOBIFLIGHT_TYPE "MobiFlight Mega"sv
#define MOBIFLIGHT_SERIAL "SN-eaa-42f"sv
#define MOBIFLIGHT_NAME "MobiFlight Mega"sv

#define FLASH_USER_DATA_START 0x080FF800
#define FLASH_USER_DATA_SIZE 2048

// https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32L4/TARGET_STM32L476xG/TARGET_NUCLEO_L476RG/PinNames.h
// Also see https://os.mbed.com/platforms/ST-Nucleo-L476RG/ for a diagram.
// Maps Arduino pin names to STM32 pins.
const map<int, PinName> PinMappings = {
    {0, PA_3},
    {1, PA_2},
    {2, PA_10},
    {3, PB_3}, // PWM
    {4, PB_5},
    {5, PB_4},  // PWM
    {6, PB_10}, // PWM
    {7, PA_8},
    {8, PA_9},
    {9, PC_7},  // PWM
    {10, PB_6}, // PWM
    {11, PA_7}, // PWM
    {12, BUTTON1},
    {13, PA_5}, // PWM, onboard LED
    {14, PB_9},
    {15, PB_8}};

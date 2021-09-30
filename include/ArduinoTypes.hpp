/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

#define ARDUINO_PIN uint8_t

// MobiFlight sends a value between 0 and 255 but mbed os
// requires a value between 0 and 1 for PWM output. For the PWM
// case divide by 255 to get the percentage to send out.
#define CONVERT_TO_MBED_PWM_VALUE(value) ((float)value / (float)255)

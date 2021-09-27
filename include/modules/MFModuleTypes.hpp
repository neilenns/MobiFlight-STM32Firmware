/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

enum class MFModuleType
{
  kNotSet,              // 0
  kButton,              // 1
  kEncoderSingleDetent, // 2 (retained for backwards compatibility, use kEncoder for new configs)
  kOutput,              // 3
  kLedSegment,          // 4
  kStepperDeprecated,   // 5 (keep for backwards compatibility, doesn't support autohome)
  kServo,               // 6
  kLcdDisplayI2C,       // 7
  kEncoder,             // 8
  kStepper,             // 9 (new stepper  with auto zero support if btnPin is > 0)
  kShiftRegister,       // 10 Shift register support (example: 74HC595, TLC592X)
  kAnalogInput          // 11 Analog Device with 1 pin
};

template <typename MFModuleType>
auto as_integer(MFModuleType const value)
    -> typename std::underlying_type<MFModuleType>::type
{
  return static_cast<typename std::underlying_type<MFModuleType>::type>(value);
}
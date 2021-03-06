/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Neil Enns. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#pragma once

// This is the list of recognized commands. These can be commands that can either be sent or received.
// In order to receive, attach a callback function to these events
//
// If you increase this list, make sure to check that the MAXCALLBACKS value
// in CmdMessenger.h is set apropriately
/**
 * @brief The list of commands recognized by MobiFlight.
 * 
 */
enum MFCommand
{
  kInitModule,           // 0
  kSetModule,            // 1
  kSetPin,               // 2
  kSetStepper,           // 3
  kSetServo,             // 4
  kStatus,               // 5, Command to report status
  kEncoderChange,        // 6
  kButtonChange,         // 7
  kStepperChange,        // 8
  kGetInfo,              // 9
  kInfo,                 // 10
  kSetConfig,            // 11
  kGetConfig,            // 12
  kResetConfig,          // 13
  kSaveConfig,           // 14
  kConfigSaved,          // 15
  kActivateConfig,       // 16
  kConfigActivated,      // 17
  kSetPowerSavingMode,   // 18
  kSetName,              // 19
  kGenNewSerial,         // 20
  kResetStepper,         // 21
  kSetZeroStepper,       // 22
  kTrigger,              // 23
  kResetBoard,           // 24
  kSetLcdDisplayI2C,     // 25
  kSetModuleBrightness,  // 26
  kSetShiftRegisterPins, // 27
  kAnalogChange,         // 28
  kTest,                 // 29
};
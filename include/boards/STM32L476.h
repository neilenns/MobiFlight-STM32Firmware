#pragma once

#include <map>
#include <mbed.h>

#define MOBIFLIGHT_TYPE "MobiFlight Mega"
#define MOBIFLIGHT_SERIAL "SN-eaa-42f"
#define MEMLEN_NAME "MobiFlight Mega"
#define EEPROM_SIZE 1024
#define MEMLEN_CONFIG 256

#define MODULE_MAX_PINS 69
#define MAX_OUTPUTS 40
#define MAX_BUTTONS 68
#define MAX_LEDSEGMENTS 4
#define MAX_ENCODERS 20
#define MAX_STEPPERS 10
#define MAX_MFSERVOS 10
#define MAX_MFLCD_I2C 2
#define MAX_ANALOG_INPUTS 5
#define MAX_SHIFTERS 10

// https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_STM/TARGET_STM32L4/TARGET_STM32L476xG/TARGET_NUCLEO_L476RG/PinNames.h
// Maps Arduino pin names to STM32 pins
const map<int, PinName> PinMappings = {
    {2, LED1},
    {3, BUTTON1},
    {4, PA_1},
    {5, PA_2},
    {6, PA_3},
    {7, PA_4},
    {8, PA_5},
    {9, PA_6},
    {10, PA_7},
};
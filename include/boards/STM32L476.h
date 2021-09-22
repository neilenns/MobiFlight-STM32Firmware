#pragma once

#include <map>
#include <mbed.h>

#define MOBIFLIGHT_TYPE "MobiFlight Mega"
#define MOBIFLIGHT_SERIAL "0987654321"
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

const map<int, PinName> PinMappings = {
    {1, LED1}};
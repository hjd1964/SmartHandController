// Platform setup ------------------------------------------------------------------------------------
// Teensy3.6
#pragma once

// We define a more generic symbol, in case more Teensy boards based on different lines are supported
#define __TEENSYDUINO__

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire1
#ifndef HAL_WIRE_CLOCK
  #define HAL_WIRE_CLOCK 100000
#endif

// Non-volatile storage ------------------------------------------------------------------------------
#if NV_DRIVER == NV_DEFAULT
  #include "../lib/nv/NV_EEPROM.h"
  #define HAL_NV_INIT() { nv.init(E2END + 1, true, 0, false); }
#endif

//----------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { analogReadResolution(10); }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation

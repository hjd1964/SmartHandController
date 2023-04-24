// -------------------------------------------------------------------------------------------------
// Loads pinmap model for current configuration
#pragma once

#include <Arduino.h>

#include "../Constants.h"
#include "../locales/Locales.h"
#include "../../Config.h"
#include "../HAL/HAL.h"
#include "../locales/Locale.h"

// Default pin state; true for active LOW, false if active HIGH
#ifndef B_PIN0_ACTIVE_STATE
  #define B_PIN0_ACTIVE_STATE LOW
#endif
#ifndef B_PIN1_ACTIVE_STATE
  #define B_PIN1_ACTIVE_STATE LOW
#endif
#ifndef B_PIN2_ACTIVE_STATE
  #define B_PIN2_ACTIVE_STATE LOW
#endif
#ifndef B_PIN3_ACTIVE_STATE
  #define B_PIN3_ACTIVE_STATE LOW
#endif
#ifndef B_PIN4_ACTIVE_STATE
  #define B_PIN4_ACTIVE_STATE LOW
#endif
#ifndef B_PIN5_ACTIVE_STATE
  #define B_PIN5_ACTIVE_STATE LOW
#endif
#ifndef B_PIN6_ACTIVE_STATE
  #define B_PIN6_ACTIVE_STATE LOW
#endif

#include "Pins.Esp32.h"
#include "Pins.Esp8266.h"
#include "Pins.M0.h"
#include "Pins.Teensy.h"

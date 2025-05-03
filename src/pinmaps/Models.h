// -------------------------------------------------------------------------------------------------
// Loads pinmap model for current configuration
#pragma once

#include <Arduino.h>

#include "../Constants.h"
#include "../locales/Locales.h"
#include "../../Config.h"
#include "../HAL/HAL.h"
#include "../locales/Locale.h"

#if defined(ESP32)
  #define PINMAP_STR "ESP32"
  #include "Pins.Esp32.h"
#elif defined(ARDUINO_TEENSY32)
  #define PINMAP_STR "TEENSY3.2"
  #include "Pins.Teensy.h"
#elif defined(ARDUINO_TEENSY35)
  #define PINMAP_STR "TEENSY3.5"
  #include "Pins.Teensy.h"
#elif defined(ARDUINO_TEENSY36)
  #define PINMAP_STR "TEENSY3.6"
  #include "Pins.Teensy.h"
#elif defined(ARDUINO_TEENSY40)
  #define PINMAP_STR "TEENSY4.0"
  #include "Pins.Teensy.h"
#elif defined(ARDUINO_TEENSY41)
  #define PINMAP_STR "TEENSY4.1"
  #include "Pins.Teensy.h"
#else
  #error "Configuration (Arduino): select either a Teensy4.0 or ESP32 device"
#endif

// all unassigned pins OFF
#include "Pins.defaults.h"

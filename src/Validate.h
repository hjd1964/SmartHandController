// -------------------------------------------------------------------------------------------------
// Validate configuration
#pragma once
#include "Common.h"

#if DISPLAY_LANGUAGE < 1 || DISPLAY_LANGUAGE > 6
  #error "Configuration (Config.h): DISPLAY_LANGUAGE is unknown"
#endif

#if DISPLAY_OLED < 0 || DISPLAY_OLED > 6
  #error "Configuration (Config.h): DISPLAY_OLED is unknown"
#endif

#if SERIAL_ONSTEP == OFF && SERIAL_RADIO == OFF
  #error "Configuration (Config.h): either SERIAL_ONSTEP and SERIAL_RADIO must be enabled"
#endif

#if SERIAL_ONSTEP_BAUD_DEFAULT != 9600 && SERIAL_ONSTEP_BAUD_DEFAULT != 19200 && SERIAL_ONSTEP_BAUD_DEFAULT != 38400 && \
    SERIAL_ONSTEP_BAUD_DEFAULT != 57600 && SERIAL_ONSTEP_BAUD_DEFAULT != 115200 && SERIAL_ONSTEP_BAUD_DEFAULT != 230400 && \
    SERIAL_ONSTEP_BAUD_DEFAULT != 460800 && SERIAL_ONSTEP_BAUD_DEFAULT != 921600 && SERIAL_ONSTEP_BAUD_DEFAULT != OFF
  #warning "Configuration (Config.h): Setting SERIAL_ONSTEP_BAUD_DEFAULT unknown, use 9600, 19200, 38400, 57600, 115200, 230400, 460800, or 921600 (baud.)"
#endif

#if SERIAL_RADIO != OFF && SERIAL_RADIO != BLUETOOTH && SERIAL_RADIO != WIFI_STATION && SERIAL_RADIO != BOTH
  #warning "Configuration (Config.h): Setting SERIAL_RADIO unknown, use OFF or BLUETOOTH or WIFI_STATION or BOTH"
#endif

#if UTILITY_LIGHT != OFF && (UTILITY_LIGHT < 0 || UTILITY_LIGHT > 255)
  #warning "Configuration (Config.h): Setting UTILITY_LIGHT unknown, use OFF or a value 0 to 255"
#endif

#if UTILITY_LIGHT != OFF && (UTILITY_LIGHT < 0 || UTILITY_LIGHT > 255)
  #warning "Configuration (Config.h): Setting UTILITY_LIGHT unknown, use OFF or a value 0 to 255"
#endif

#if WEATHER != OFF && (WEATHER < WEATHER_FIRST || WEATHER > WEATHER_LAST)
  #error "Configuration (Config.h): Setting WEATHER unknown, use OFF or valid WEATHER SENSOR (from Constants.h)"
#endif

#if DISPLAY_AMBIENT_CONDITIONS != OFF && DISPLAY_AMBIENT_CONDITIONS != ON
  #error "Configuration (Config.h): Setting DISPLAY_AMBIENT_CONDITIONS unknown, use OFF or ON"
#endif

#if DISPLAY_WRAP_MENUS != OFF && DISPLAY_WRAP_MENUS != ON
  #error "Configuration (Config.h): Setting DISPLAY_WRAP_MENUS unknown, use OFF or ON"
#endif

#if DISPLAY_24HR_TIME != OFF && DISPLAY_24HR_TIME != ON
  #error "Configuration (Config.h): Setting DISPLAY_24HR_TIME unknown, use OFF or ON"
#endif

#if DISPLAY_CONTRAST_DEFAULT < 0 || DISPLAY_CONTRAST_DEFAULT > 3
  #warning "Configuration (Config.h): Setting DISPLAY_CONTRAST_DEFAULT unknown, use a value 0 to 3 (for min, low, high, or max)"
#endif

#if KEYPAD_JOYSTICK_ANALOG != OFF && KEYPAD_JOYSTICK_ANALOG != JS1
  #error "Configuration (Config.h): Setting KEYPAD_JOYSTICK_ANALOG unknown, use OFF or JS1"
#endif

#if KEYPAD_JOYSTICK_HYSTERESIS < 0 || KEYPAD_JOYSTICK_HYSTERESIS > 1023
  #warning "Configuration (Config.h): Setting KEYPAD_JOYSTICK_HYSTERESIS unknown, use a value 0 to 1023 (default analog read range)"
#endif

#if ST4_AUX_INTERFACE != OFF && ST4_AUX_INTERFACE != ON
  #error "Configuration (Config.h): Setting ST4_AUX_INTERFACE unknown, use OFF or ON"
#endif

// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                    "SHC" // Friendly three letter product abbreviation

#define METRIC                      -200
#define IMPERIAL                    -201
#define LOCALE_DEFAULT              -202
#define AUTO_ON                     -203
#define AUTO_OFF                    -204
#define FWU                         -205
#define JS1                         -206

#define TIMEOUT_CMD                 2000  // Default 2000 ms, 2 seconds
#define FOREGROUND_CMD_RATE         300   // Default 300 ms, regulates the forground command rate
#define BACKGROUND_CMD_RATE         1500  // Default 1500 ms, regulates the background (dimmed display) command rate

// AUXILIARY FEATURE purpose
#define AUX_FEATURE_PURPOSE_FIRST   1
#define SWITCH                      1      // control an simple on/off switch
#define ANALOG_OUTPUT               2      // control an analog (pwm) output, depends on MCU support
#define ANALOG_OUT                  2
#define DEW_HEATER                  3      // control an dew heater
#define INTERVALOMETER              4      // control an camera shutter
#define AUX_FEATURE_PURPOSE_LAST    4

// NV -------------------------------------------------------------------------------------------------------------------
#define INIT_NV_KEY                 159301352UL

#define NV_KEY                      0      // bytes: 4   , addr:   0..  3
#define NV_DISPLAY_SETTINGS_BASE    4      // bytes: 1   , addr:   4..  4

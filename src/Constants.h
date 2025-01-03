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

// WEATHER sensors (temperature, pressure, and humidity)
#define WEATHER_FIRST               1
#define BME280                      1      // BME280 on I2C (at default address 0x77)
#define BME280_0x77                 1      // BME280 on I2C (at address 0x77)
#define BME280_0x76                 2      // BME280 on I2C (at address 0x76)
#define BME280_SPI                  3      // BME280 on SPI (default CS)
#define BMP280                      4      // BMP280 on I2C (at default address 0x77)
#define BMP280_0x77                 4      // BMP280 on I2C (at address 0x77)
#define BMP280_0x76                 5      // BMP280 on I2C (at address 0x76)
#define BMP280_SPI                  6      // BMP280 on SPI (default CS)
#define WEATHER_LAST                6

// NV -------------------------------------------------------------------------------------------------------------------
#define INIT_NV_KEY                 159301352UL

#define NV_KEY                      0      // bytes: 4   , addr:   0..  3
#define NV_DISPLAY_SETTINGS_BASE    4      // bytes: 1   , addr:   4..  4

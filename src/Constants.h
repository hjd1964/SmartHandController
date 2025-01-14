// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define TASKS_HWTIMERS              1

#define METRIC                      -200
#define IMPERIAL                    -201
#define LOCALE_DEFAULT              -202
#define AUTO_ON                     -203
#define AUTO_OFF                    -204
#define FWU                         -205
#define JS1                         -206

#define TIMEOUT_CMD                 3000  // Default 3000 ms, 3 seconds
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
#define INIT_NV_KEY                 159301359UL

#define NV_KEY                      0      // bytes:   4 , addr:   0..  3
#define NV_DISPLAY_SETTINGS_BASE    4      // bytes:  24 , addr:   4.. 27
#define NV_SERIAL_BOOT_FLAG_BASE   28      // bytes:   1 , addr:  28.. 28
#define NV_BT_SETTINGS_BASE        29      // bytes: 149 , addr:  29..177 (BluetoothSettingsSize)
#define NV_WIFI_SETTINGS_BASE     178      // bytes: 790 , addr: 178..968 (WifiSettingsSize)

// ---------------------------------------------------------------------------------------------------------------------------------
// Configuration for OnStep Smart Hand Controller (SHC)

/*
 *          For more information on setting OnStep up see http://www.stellarjourney.com/index.php?r=site/equipment_onstep 
 *                      and join the OnStep Groups.io at https://groups.io/g/onstep
 * 
 *           *** Read the compiler warnings and errors, they are there to help guard against invalid configurations ***
*/

// ---------------------------------------------------------------------------------------------------------------------------------
// ADJUST THE FOLLOWING TO CONFIGURE YOUR ADD-ON'S FEATURES ------------------------------------------------------------------------
// <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed

//      Parameter Name              Value   Default  Notes                                                                      Hint
// DISPLAY -------------------------------------------------------------------------------------------------------------------------
#define DISPLAY_LANGUAGE             L_en //   L_en, English. Specify language with two letter country code, if supported.    Adjust
#define DISPLAY_OLED               SH1106 // SH1106, OLED 1.3" I2C display commonly used. SSD1306 is a 0.96" OLED display.    Infreq
                                          //         The SSD1309 is often used on 1.54" or 2.3" I2C displays.

// SERIAL PORTS --------------------------------------------------------------------------------------------------------------------
#define SERIAL_ONSTEP          SERIAL_ST4 // .._ST4, SERIAL_ST4 for ST4 port sync comms, you can also use any other available Infreq
                                          //         serial port (if pins are unused,) Serial3 on the Teensy3.2 or SERIAL_IP
                                          //         for example.  This is the serial interface connected to OnStep.
#define SERIAL_ONSTEP_BAUD_DEFAULT   9600 //   9600, Common baud rates for these parameters are 9600,19200,57600,115200.      Infreq
                                          //         Only used for async serial communication with OnStep.

// USER FEEDBACK -------------------------------------------------------------------------------------------------------------------
#define UTILITY_LIGHT                 OFF //    OFF, n. Where n=0..255 (0..100%) activates feature sets default brightness.   Adjust

// WEATHER SENSOR ------------------------------------------------------------------------------------------------------------------
#define WEATHER                      OFF //    OFF, BME280 (I2C 0x77,) BME280_0x76, BMP280 (I2C 0x77,) BMP280_0x76            Option
                                         //         BME280 or BMP280 for temperature, pressure.  BME280 for humidity also.

// DISPLAY -------------------------------------------------------------------------------------------------------------------------
#define DISPLAY_AMBIENT_CONDITIONS    OFF //    OFF, ON to show ambient conditions in the display rotation                    Option
#define DISPLAY_WRAP_MENUS            OFF //    OFF, ON allows menus to wrap so moving past bottom returns to top, etc.       Option
#define DISPLAY_24HR_TIME              ON //     ON, OFF to use 12 hour format for entering time.                             Option

// KEYPAD --------------------------------------------------------------------------------------------------------------------------
#define KEYPAD_JOYSTICK_ANALOG        OFF //    OFF, JS1 for Jerry's analog joystick                                          Option

// ST4 AUXILIARY INTERFACE ---------------------------------------------------------------------------------------------------------
// *** It is up to you to verify the interface meets the electrical specifications of any connected device, use at your own risk ***
#define ST4_AUX_INTERFACE             OFF //    OFF, ON, ON_PULLUP enables auxillary "pass-through" ST4 interface.            Option

// CATALOGS ------------------------------------------------------------------------------------------------------------------------
// Advanced users can also customize which celestial object catalogs are included or even add new catalogs using Config.catalogs.h

// THAT'S IT FOR USER CONFIGURATION!

// ---------------------------------------------------------------------------------------------------------------------------------
#include "Extended.config.h"

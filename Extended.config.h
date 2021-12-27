// ---------------------------------------------------------------------------------------------------------------------------------
// Configuration for OnStep SmartHandServer infrequently used eXtended options

//                                 *** Most users will not change anything in this file! ***

// ---------------------------------------------------------------------------------------------------------------------------------
// ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES ----------------------------------------------------------------------
// <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed

// DEBUG ------------------------------------------------------------ see https://onstep.groups.io/g/main/wiki/6-Configuration#DEBUG
// Enable additional debugging and/or status messages on the specified SERIAL_DEBUG port
// Note that the SERIAL_DEBUG port cannot be used for normal communication with OnStep

//      Parameter Name              Value        Default  Notes                                                                 Hint
#define DEBUG                         OFF //         OFF, Use ON for background error messages only, use VERBOSE for all      Infreq
                                          //              error and status messages, use PROFILER for VT100 task profiler,
                                          //              use REMOTE to forward verbose debug output to OnStep
#define SERIAL_DEBUG               Serial //      Serial, Or use any h/w serial port. Serial1 or Serial2, etc. as supported.  Option
#define SERIAL_DEBUG_BAUD            9600 //        9600, n. Where n=9600,19200,57600,115200 (common baud rates.)             Option

// IP ADDRESS SETTINGS -------------------------------------------------------------------------------------------------------------

#define SERIAL_IP_MODE                OFF //         OFF, Use STATION to enable the interface (ESP32 only.)                   Option
                                          //              then specify SERIAL_IP as the serial port in Config.h

#define ONSTEP1_HOST_NAME        "OnStep" //    "OnStep", Wifi Host Name to connnect to, leave blank to disable.              Adjust
#define ONSTEP1_SSID             "ONSTEP" //    "ONSTEP", Wifi alternate Station SSID to connnect to.                         Adjust
#define ONSTEP1_PASSWORD       "password" //  "password", Wifi Station mode password.                                         Adjust
#define ONSTEP1_IP_ADDR     {192,168,0,1} //  ..168,0,1}, Wifi Target IP Address to connect to.                               Adjust

#define ONSTEP2_HOST_NAME              "" //          "", Wifi Host Name to connnect to, leave blank to disable.              Adjust
#define ONSTEP2_SSID             "ONSTEP" //    "ONSTEP", Wifi alternate Station SSID to connnect to.                         Adjust
#define ONSTEP2_PASSWORD       "password" //          "", Wifi alternate Station mode password.                               Adjust
#define ONSTEP2_IP_ADDR     {192,168,0,1} //  ..168,0,1}, Wifi alternate Target IP Address to connect to.                     Adjust

#define ONSTEP3_HOST_NAME              "" //          "", Wifi Host Name to connnect to, leave blank to disable.              Adjust
#define ONSTEP3_SSID             "ONSTEP" //    "ONSTEP", Wifi alternate Station SSID to connnect to.                         Adjust
#define ONSTEP3_PASSWORD       "password" //  "password", Wifi alternate Station mode password.                               Adjust
#define ONSTEP3_IP_ADDR     {192,168,0,1} //  ..168,0,1}, Wifi alternate Target IP Address to connect to.                     Adjust

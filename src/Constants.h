// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define METRIC                      -200
#define IMPERIAL                    -201
#define LOCALE_DEFAULT              -202
#define AUTO_ON                     -203
#define AUTO_OFF                    -204
#define FWU                         -205
#define JS1                         -206

#define TIMEOUT_CMD                 1000  // Default 1000 ms, 1 second
#define BACKGROUND_CMD_RATE         300   // Default 300 ms, regulates the background command rate

// a code to identify the SHC
#define initKey                     59301352

#define EE_autoInitKey              0
#define EE_reticule                 4
#define EE_dispMaxContrast          8
#define EE_dispDimTimeout           12
#define EE_dispBlankTimeout         16

#define Abv                         "SHC: "

// activate ST4 port serial interface
#define SERIAL_ST4
#define SERIAL_ST4_SLAVE_PRESENT

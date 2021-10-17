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

#define Abv                         "SHC: "

// NV -------------------------------------------------------------------------------------------------------------------
#define INIT_NV_KEY                 159301352UL

#define NV_KEY                      0      // bytes: 4   , addr:   0..  3
#define NV_DISPLAY_SETTINGS_BASE    4      // bytes: 1   , addr:   4..  4

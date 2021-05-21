// -----------------------------------------------------------------------------------
// Constants
#pragma once

// on/off, etc.
#define OFF                    -1
#define ON                     -2
#define METRIC                 ON
#define IMPERIAL               -3
#define LOCALE_DEFAULT         -4
#define AUTO                   -5
#define AUTO_ON                -6
#define AUTO_OFF               -7
#define FWU                    -8
#define VERBOSE                -9
#define REMOTE                -10
#define JS1                   -11

#define TIMEOUT_CMD           1000      // Default 1000 ms, 1 second
#define BACKGROUND_CMD_RATE   300       // Default 300 ms, regulates the background command rate

#define initKey               59301352  // a code to identify the SHC

#define EE_autoInitKey        0
#define EE_reticule           4
#define EE_dispMaxContrast    8
#define EE_dispDimTimeout     12
#define EE_dispBlankTimeout   16

#define NV_DEFAULT

#define Abv                   "SHC: "
#define FirmwareDate          __DATE__
#define FirmwareTime          __TIME__

#pragma once

// Enable debug and/or status messages to be passed to OnStep for display using its debug facilities
// default "DEBUG OFF", use "DEBUG ON" for background errors only, use "DEBUG VERBOSE" for all errors and status messages
#define DEBUG VERBOSE

// on/off, etc.
#define OFF -1
#define ON  -2
#define JS1 -3

#define TIMEOUT_CMD         1000  // Default=1000 (1 second)
#define BACKGROUND_CMD_RATE 300   // Default=300, regulates the background command rate

#define initKey 59301350  // a code to identify the SHC

#define EE_autoInitKey       0
#define EE_reticule          4
#define EE_dispMaxContrast   8
#define EE_dispDimTimeout   12
#define EE_dispBlankTimeout 16

#define NV_DEFAULT

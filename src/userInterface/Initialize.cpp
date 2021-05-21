// -----------------------------------------------------------------------------------
// Initialize, get settings from NV on startup

#include "UserInterface.h"
extern NVS nv;

void UI::initNV() {
  // EEPROM automatic initialization
  long autoInitKey = initKey;
  long thisAutoInitKey;
  thisAutoInitKey = nv.readL(EE_autoInitKey);
  if (autoInitKey != thisAutoInitKey) {

    // stop the init from happening again
    nv.write(EE_autoInitKey, (int32_t)autoInitKey);

    nv.write(EE_dispMaxContrast, (int32_t)1);
    nv.write(EE_dispDimTimeout, (int32_t)2);
    nv.write(EE_dispBlankTimeout, (int32_t)3);
  }
}

void UI::readNV() {
  maxContrast = (nv.readL(EE_dispMaxContrast)) * 63;
  display_dim_time = (nv.readL(EE_dispDimTimeout) - 1) * 30000;
  display_blank_time = (nv.readL(EE_dispBlankTimeout) - 1) * 60 * 1000;
}

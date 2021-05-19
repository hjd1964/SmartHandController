// -----------------------------------------------------------------------------------
// Functions for variables on startup

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

    nv.write(EE_dispMaxContrast, (int32_t)maxContrast);
    nv.write(EE_dispDimTimeout, (int32_t)display_dim_time);
    nv.write(EE_dispBlankTimeout, (int32_t)display_blank_time);
  }
}

void UI::readNV() {
  #ifndef DISABLE_EEPROM_COMMIT_ON
    maxContrast = nv.readL(EE_dispMaxContrast);
    display_dim_time = nv.readL(EE_dispDimTimeout);
    display_blank_time = nv.readL(EE_dispBlankTimeout);
  #endif
}

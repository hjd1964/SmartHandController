#pragma once

#include <U8g2lib.h>
#include "u8g2_ext_event.h"

bool ext_UserInterfaceCatalog(u8g2_t *u8g2, KeyPad *extPad, const char *title);
bool ext_UserInterfaceUserCatalog(u8g2_t *u8g2, KeyPad *extPad, const char *title);

//#ifdef ESP32
//  #define u8g2_GetAscentEx(x) (u8g2_GetAscent(x) - 3)
//#else
  #define u8g2_GetAscentEx(x) u8g2_GetAscent(x)
//#endif

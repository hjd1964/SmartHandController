#pragma once

#include <U8g2lib.h>
#include "u8g2_ext_event.h"

#include "../../Common.h"

bool ext_UserInterfaceCatalog(u8g2_t *u8g2, KeyPad *extPad, const char *title);
bool ext_UserInterfaceUserCatalog(u8g2_t *u8g2, KeyPad *extPad, const char *title);

#pragma once

#include "../keyPad/KeyPad.h"

#define MSG_MENU_UP_FAST 240
#define MSG_MENU_DOWN_FAST 241

uint8_t ext_GetMenuEvent(KeyPad* extPad);

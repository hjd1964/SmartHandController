#pragma once

#include <U8g2lib.h>
#include "u8g2_ext_event.h"

uint8_t ext_UserInterfaceSelectionList(u8g2_t *u8g2, KeyPad *extPad, const char *title, uint8_t start_pos, const char *sl, bool wrap);

//#ifdef ESP32
//  #define u8g2_GetAscentEx(x) (u8g2_GetAscent(x) - 3)
//  #define u8g2_OffsetY 2
//  #define MY_SELECTION_BORDER_SIZE 0
//#else
  #define u8g2_GetAscentEx(x) u8g2_GetAscent(x)
  #define u8g2_OffsetY 0
  #define MY_SELECTION_BORDER_SIZE MY_BORDER_SIZE
//#endif

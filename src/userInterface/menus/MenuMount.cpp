// -----------------------------------------------------------------------------------
// MenuMount, for UserInterface
#include "../UserInterface.h"

void UI::menuMount() {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    const char *string_list_Mount = L_MOUNT_SPEED "\n" L_MOUNT_BL "\n" L_MOUNT_LIMITS "\n" L_MOUNT_PIER;
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, L_MOUNT_CONFIG, current_selection_L2, string_list_Mount);
    switch (current_selection_L2) {
      case 1: menuGotoSpeed(); break;
      case 2: menuBacklash(); break;
      case 3: menuLimits(); break;
      case 4: menuPier(); break;
    }
  }
}

void UI::menuGotoSpeed() {
  char string_list_LimitsL2[80];
  strcpy(string_list_LimitsL2,L_MOUNT_FASTEST "\n" L_MOUNT_FASTER "\n" L_MOUNT_DEFAULT_SPEED "\n" L_MOUNT_SLOWER "\n" L_MOUNT_SLOWEST);

  char mr[20] = ""; boolean ok = onStep.GetTrim(":GX93#",mr) == CR_VALUE_GET;
  if (ok) {
    double rateDefault = atof(mr);
    ok = onStep.GetTrim(":GX92#",mr) == CR_VALUE_GET;
    if (ok) {
      double rateCurrent = atof(mr);
      double r = rateDefault/rateCurrent;
      if (r > 1.75) current_selection_L3 = 1; else
      if (r > 1.25) current_selection_L3 = 2; else
      if (r > 0.875) current_selection_L3 = 3; else
      if (r > 0.625) current_selection_L3 = 4; else current_selection_L3 = 5;
    } else current_selection_L3 = 1;
  } else current_selection_L3 = 1;

  while (current_selection_L3 != 0)
  {
    current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_MOUNT_SPEED, current_selection_L3, string_list_LimitsL2);
    switch (current_selection_L3)
    {
    case 1: onStep.Set(":SX93,1#"); message.show(L_MOUNT_SPEED, "2X", 1500); break;
    case 2: onStep.Set(":SX93,2#"); message.show(L_MOUNT_SPEED, "1.5X", 1500); break;
    case 3: onStep.Set(":SX93,3#"); message.show(L_MOUNT_SPEED, "1X", 1500); break;
    case 4: onStep.Set(":SX93,4#"); message.show(L_MOUNT_SPEED, "0.75X", 1500); break;
    case 5: onStep.Set(":SX93,5#"); message.show(L_MOUNT_SPEED, "0.5X", 1500); break;
    }
  }
}

void UI::menuBacklash()
{
  char string_list_LimitsL2[80];
  uint8_t bl;
  strcpy(string_list_LimitsL2, "Axis1 RA/Az\n""Axis2 Dec/Alt");
  current_selection_L3 = 1;
  while (current_selection_L3 != 0) {
    current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_MOUNT_BL, current_selection_L3, string_list_LimitsL2);
    switch (current_selection_L3) {
      case 1: bl = 1; menuSetBacklash(bl); break;
      case 2: bl = 2; menuSetBacklash(bl); break;
      default: break;
    }
  }
}

bool UI::menuSetBacklash(uint8_t &axis)
{
  float backlash;
  if (!message.show(onStep.readBacklash(axis, backlash))) return false;
  char text[20];
  sprintf(text, L_MOUNT_BL " Axis%u", axis);
  if (display->UserInterfaceInputValueFloat(&keyPad, text, "", &backlash, 0, 3600, 4, 0, " " L_ARCSEC)) {
    return message.show(onStep.writeBacklash(axis, backlash), false);
  }
  return true;
}

void UI::menuLimits() {
  char string_list_LimitsL2[80];
 
  if (status.isMountGEM()) {
    strcpy(string_list_LimitsL2,L_MOUNT_LIMIT_H "\n" L_MOUNT_LIMIT_O "\n" L_MOUNT_LIMIT_ME "\n" L_MOUNT_LIMIT_MW);
  } else {
    strcpy(string_list_LimitsL2,L_MOUNT_LIMIT_H "\n" L_MOUNT_LIMIT_O);
  }
  
  current_selection_L3 = 1;
  while (current_selection_L3 != 0) {
    current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_MOUNT_LIMITS, current_selection_L3, string_list_LimitsL2);
    switch (current_selection_L3) {
      case 1:
        menuHorizon();
      break;
      case 2:
        menuOverhead();
      break;
      case 3:
        menuMeridianE();
      break;
      case 4:
        menuMeridianW();
      break;
    }
  }
}

void UI::menuHorizon() {
  char out[20];
  if (message.show(onStep.Get(":Gh#", out))) {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&keyPad, L_MOUNT_LIMIT_HORIZON, "", &angle, -10, 20, 2, 0, " degree")) {
      sprintf(out, ":Sh%+03d#", (int)angle);
      message.show(onStep.Set(out), false);
    }
  }
}

void UI::menuOverhead() {
  char out[20];
  if (message.show(onStep.Get(":Go#", out))) {
    float angle = (float)strtol(&out[0], NULL, 10);
    if (display->UserInterfaceInputValueFloat(&keyPad, L_MOUNT_LIMIT_OVERHEAD, "", &angle, 60, 91, 2, 0, " " L_DEGREE)) {
      sprintf(out, ":So%02d#", (int)angle);
      message.show(onStep.Set(out), false);
    }
  }
}

void UI::menuMeridianE() {
  char out[20] = "";
  if (message.show(onStep.Get(":GXE9#", out))) {
    float angle = (float)strtol(&out[0], NULL, 10);
    angle = round((angle * 15.0) / 60.0);
    if (display->UserInterfaceInputValueFloat(&keyPad, L_MOUNT_LIMIT_MERIDIAN_EAST, "", &angle, -180, 180, 3, 0, " " L_DEGREE)) {
      angle = round((angle * 60.0) / 15.0);
      sprintf(out, ":SXE9,%+02d#", (int)angle);
      message.show(onStep.Set(out), false);
    }
  }
}

void UI::menuMeridianW() {
  char out[20] = "";
  if (message.show(onStep.Get(":GXEA#", out))) {
    float angle = (float)strtol(&out[0], NULL, 10);
    angle = round((angle * 15.0) / 60.0);
    if (display->UserInterfaceInputValueFloat(&keyPad, L_MOUNT_LIMIT_MERIDIAN_WEST, "", &angle, -180, 180, 3, 0, " " L_DEGREE)) {
      angle = round((angle * 60.0) / 15.0);
      sprintf(out, ":SXEA,%+02d#", (int)angle);
      message.show(onStep.Set(out), false);
    }
  }
}

void UI::menuPier() {
  // get preferred pier side user setting
  bool ok = false;
  char ppsState[20]=""; ok = onStep.Get(":GX96#",ppsState) == CR_VALUE_GET;
  if (ok) {
    uint8_t choice = 1;
    if (ppsState[0] == 'B') choice = 1; else
    if (ppsState[0] == 'E') choice = 2; else
    if (ppsState[0] == 'W') choice = 3;
    
    choice = display->UserInterfaceSelectionList(&keyPad, L_MOUNT_PPS, choice, L_PPS_BEST "\n" L_PPS_EAST "\n" L_PPS_WEST);
    if (choice) {
      if (choice == 1) ok = message.show(onStep.Set(":SX96,B#"), false); else
      if (choice == 2) ok = message.show(onStep.Set(":SX96,E#"), false); else
      if (choice == 3) ok = message.show(onStep.Set(":SX96,W#"), false);
    }
  }
}

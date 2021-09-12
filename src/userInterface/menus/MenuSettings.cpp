// -----------------------------------------------------------------------------------
// MenuSettings, for UserInterface
#include "../UserInterface.h"
extern NVS nv;

void UI::menuSettings() {
  current_selection_L1 = 1;
  while (current_selection_L1 != 0) {
    char string_list_SettingsL1[150] = "";

    int i = 1;
    int index[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    index[1] = i++; strcat(string_list_SettingsL1,L_SET_DATE_TIME);
    index[2] = i++; strcat(string_list_SettingsL1,"\n" L_SET_SITE);
    index[3] = i++; strcat(string_list_SettingsL1,"\n" L_SET_DISPLAY);
    index[4] = i++; strcat(string_list_SettingsL1,"\n" L_SET_BUZ);
    if (status.isMountGEM()) { index[5] = i++; strcat(string_list_SettingsL1,"\n" L_SET_MERIDIAN_FLIP); }
    index[6] = i++; strcat(string_list_SettingsL1,"\n" L_SET_CONFIG);
    index[7] = i++; strcat(string_list_SettingsL1,"\n" L_SET_VERSION);
    if (status.hasRotator())  { index[8] = i++; strcat(string_list_SettingsL1,"\n" L_SET_ROTATOR); }
    char nums[] = "0";
    for (int n = 0; n < 6; n++) {
      if (status.hasFocuser(n)) {
        index[3] = i++;
        strcat(string_list_SettingsL1,"\n" L_SET_FOCUSER);
        nums[0] = '1' + n;
        if (status.getFocuserCount() > 1) strcat(string_list_SettingsL1, nums);
      }
    }

    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, L_SETTINGS, current_selection_L1, string_list_SettingsL1);
    if (current_selection_L1 == index[1]) menuLocalDateTime();
    if (current_selection_L1 == index[2]) menuSite();
    if (current_selection_L1 == index[3]) menuDisplay();
    if (current_selection_L1 == index[4]) menuSound();
    if (current_selection_L1 == index[5]) menuMeridianFlips();
    if (current_selection_L1 == index[6]) menuMount();
    if (current_selection_L1 == index[7]) menuFirmware();
    if (current_selection_L1 == index[8]) menuRotator();
    if (current_selection_L1 == index[9]) menuFocuser(1);
    if (current_selection_L1 == index[10]) menuFocuser(2);
    if (current_selection_L1 == index[11]) menuFocuser(3);
    if (current_selection_L1 == index[12]) menuFocuser(4);
    if (current_selection_L1 == index[13]) menuFocuser(5);
    if (current_selection_L1 == index[14]) menuFocuser(6);
  }
}

void UI::menuLocalDateTime() {
  char out[20];
  // Date
  if (DisplayMessageLX200(GetLX200(":GC#", out))) {
    char* pEnd;
    uint8_t month = strtol(&out[0], &pEnd, 10);
    uint8_t day = strtol(&out[3], &pEnd, 10);
    uint8_t year = strtol(&out[6], &pEnd, 10);
    if (display->UserInterfaceInputValueDate(&buttonPad, L_SET_LOCAL_DATE, year, month, day)) {
      sprintf(out, ":SC%02d/%02d/%02d#", month, day, year); DisplayMessageLX200(SetLX200(out),false);
      // Time
      long value;
      boolean pmf=false;
      boolean dst=false;
      if (DisplayMessageLX200(GetTimeLX200(value))) {
        if ((!hrs24) && (value>=43200)) { value-=43200; pmf=true; }
        if (display->UserInterfaceInputValueTime(&buttonPad, &value, hrs24)) {
          if (hrs24 || (display->UserInterfaceInputValueBoolean(&buttonPad, L_SET_LOCAL_PM "?", &pmf))) {
            if (pmf) value+=43200; // AM or PM?
            if (display->UserInterfaceInputValueBoolean(&buttonPad, L_SET_LOCAL_DST "?", &dst)) {
              if (dst) value-=3600; // Dst?
              DisplayMessageLX200(SetTimeLX200(value),false);
            }
          }
        }
      }
    }
  }
}

void UI::menuDisplay() {
  const char *string_list_Display = L_SET_DISP_OFF "\n" L_SET_DISP_CONT "\n" L_SET_DISP_DIM_TO "\n" L_SET_DISP_BLANK_TO;
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, L_DISPLAY, current_selection_L2, string_list_Display);
    switch (current_selection_L2) {
      case 1:
        DisplayMessage(L_SET_DISP_MSG1, L_SET_DISP_MSG2, 1500);
        sleepDisplay = true;
        display->sleepOn();
        current_selection_L2 = 0;
        current_selection_L1 = 0;
        current_selection_L0 = 0;
      break;
      case 2:
        menuContrast();
      break;
      case 3:
        menuDimTimeout();
      break;
      case 4:
        menuBlankTimeout();
      break;
    }
  }
}

void UI::menuContrast() {
  const char *string_list_Display = L_SET_DISP_MIN "\n" L_SET_DISP_LOW "\n" L_SET_DISP_HIGH "\n" L_SET_DISP_MAX;
  current_selection_L3 = nv.readL(EE_dispMaxContrast);
  
  current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, L_SET_DISP_CONTRAST, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    maxContrast = UI::Contrast[current_selection_L3 - 1];
    display->setContrast(maxContrast);
    nv.write(EE_dispMaxContrast, (int32_t)current_selection_L3);
  }
}

void UI::menuDimTimeout() {
  const char *string_list_Display = L_DISABLE "\n""30 " L_SEC_ABV "\n""60 " L_SEC_ABV;
  current_selection_L3 = nv.readL(EE_dispDimTimeout);

  current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, L_SET_DISP_DIM_TO, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    display_dim_time = (current_selection_L3 - 1) * 30000;
    nv.write(EE_dispDimTimeout, (int32_t)current_selection_L3);
  }
}

void UI::menuBlankTimeout() {
  const char *string_list_Display = L_DISABLE "\n""1 " L_MIN_ABV "\n""2 " L_MIN_ABV "\n""3 " L_MIN_ABV "\n""4 " L_MIN_ABV "\n""5 " L_MIN_ABV;
  current_selection_L3 = nv.readL(EE_dispBlankTimeout);

  current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, L_SET_DISP_BLANK_TO, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    display_blank_time = (current_selection_L3 - 1) * 60 * 1000;
    nv.write(EE_dispBlankTimeout, (int32_t)current_selection_L3);
  }
}

void UI::menuSound() {
  boolean sound = false;
  if (display->UserInterfaceInputValueBoolean(&buttonPad, L_SET_BUZZER, &sound)) {
    if (sound) DisplayMessageLX200(SetLX200(":SX97,1#"),false); else DisplayMessageLX200(SetLX200(":SX97,0#"),false);
  }
}

void UI::menuMeridianFlips() {
  const char *string_list = L_NOW "!" "\n" L_SET_MF_AUTO "\n" L_SET_MF_PAUSE;
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    boolean autoflip = false;
    boolean pause = false;
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, L_SET_MF, current_selection_L2, string_list);
    switch (current_selection_L2) {
      case 1:
        DisplayMessageLX200(SetLX200(":MN#"),false);
      break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_SET_MF_AF, &autoflip)) {
          if (autoflip) DisplayMessageLX200(SetLX200(":SX95,1#"),false); else DisplayMessageLX200(SetLX200(":SX95,0#"),false);
        }
      break;
      case 3:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_SET_MF_PF, &pause)) {
          if (pause) DisplayMessageLX200(SetLX200(":SX97,1#"),false); else DisplayMessageLX200(SetLX200(":SX97,0#"),false);
        }
      break;
    }
  }
}

void UI::menuSite() {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    const char *string_list_SiteL2 = L_SET_SITE_SELECT "\n" L_SET_SITE_LAT "\n" L_SET_SITE_LONG "\n" L_SET_SITE_UTC;
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, L_SET_SITE_TITLE, current_selection_L2, string_list_SiteL2);
    switch (current_selection_L2) {
      case 1: menuSites(); break;
      case 2: menuLatitude(); break;
      case 3: menuLongitude(); break;
      case 4: menuZone(); break;
    }
  }
}

void UI::menuSites() {
  int val;
  if (DisplayMessageLX200(GetSiteLX200(val))) {
    current_selection_L3 = val;
    const char *string_list_SiteL3 = L_SET_SITE_NUM " 1" "\n" L_SET_SITE_NUM " 2" "\n" L_SET_SITE_NUM " 3" "\n" L_SET_SITE_NUM " 4";
    current_selection_L3 = display->UserInterfaceSelectionList(&buttonPad, L_SET_SITE_NUM_TITLE, current_selection_L3, string_list_SiteL3);
    if (current_selection_L3 != 0) {
      val = current_selection_L3 - 1;
      SetSiteLX200(val);
    }
  }
}

void UI::menuLatitude() {
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gt#", out))) {
    char* pEnd;
    int degree = (int)strtol(&out[0], &pEnd, 10);
    int minute = (int)strtol(&out[4], &pEnd, 10);
    long angle = degree * 60;
    degree > 0 ? angle += minute : angle -= minute;
    angle *= 60;
    if (display->UserInterfaceInputValueLatitude(&buttonPad, &angle)) {
      angle /= 60;
      minute = abs(angle % 60);
      degree = angle / 60;
      sprintf(out, ":St%+03d*%02d#", degree, minute);
      DisplayMessageLX200(SetLX200(out),false);
    }
  }
}

void UI::menuLongitude() {
  char out[20];
  if (DisplayMessageLX200(GetLX200(":Gg#", out))) {
    char* pEnd;
    int degree = (int)strtol(&out[0], &pEnd, 10);
    int minute = (int)strtol(&out[5], &pEnd, 10);
    long angle = degree * 60;
    degree > 0 ? angle += minute : angle -= minute;
    angle *= 60;
    if (display->UserInterfaceInputValueLongitude(&buttonPad, &angle)) {
      angle /= 60;
      minute = abs(angle) % 60;
      degree = angle / 60;
      sprintf(out, ":Sg%+04d*%02d#", degree, minute);
      DisplayMessageLX200(SetLX200(out), false);
    }
  }
}

void UI::menuZone() {
  char out[20];
  if (DisplayMessageLX200(GetLX200(":GG#", out))) {
    char* pEnd;
    int hr = (int)strtol(&out[0], &pEnd, 10);

    boolean negative=false;
    if (hr<0) negative=true;
    uint8_t b=abs(hr);
  
    if (display->UserInterfaceInputValueInteger(&buttonPad, "UTC " L_OFFSET_ABV " (-" L_TZ_ABV ")", "", &b, 0, 14, 2, " " L_HRS_ABV)) {
      if (display->UserInterfaceInputValueBoolean(&buttonPad, "UTC " L_OFFSET_ABV " - ?", &negative)) {
        hr = b;
        if (negative) hr = -hr;
        sprintf(out, ":SG%+02d#", hr);
        DisplayMessageLX200(SetLX200(out), false);
      }
    }
  }
}

void UI::menuFirmware() {
  char out[20];
  
  sprintf(out,"SHC %s", _version);
  DisplayMessage(out, __DATE__, 3000);

  char temp1[20];
  char temp2[20];
  if ( (DisplayMessageLX200(GetLX200(":GVN#", temp1)))&&(DisplayMessageLX200(GetLX200(":GVD#", temp2))) )
  { for (char* p = temp1; (p = strchr(p, '#')); ++p) { *p = 0;} 
    for (char* p = temp2; (p = strchr(p, '#')); ++p) { *p = 0;} 
    sprintf(out,"OnStep %s",temp1);
    DisplayMessage(out, temp2, 3000);
  }
}

void UI::menuFocuser(uint8_t foc) {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    const char *string_list_SiteL2 = L_FOC_RET_HOME "\n" L_FOC_AT_HOME "\n" L_FOC_BACKLASH "\n" L_FOC_TC "\n" L_FOC_TC_COEF "\n" L_FOC_TC_DEADBAND;
    
    char title[40];
    if (status.getFocuserCount() > 1) sprintf(title, "%s %i", L_FOCUSER, foc); else sprintf(title, "%s", L_FOCUSER);
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, title, current_selection_L2, string_list_SiteL2);

    bool isOk = false;
    char cmd[40];
    switch (current_selection_L2) {
      case 1:
        sprintf(cmd, ":FA%u#:Fh#", foc);
        DisplayMessageLX200(SetLX200(cmd),false);
      break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_FOC_AT_HALF, &isOk)) {
          sprintf(cmd, ":FA%u#:FH#", foc);
          if (isOk) { DisplayMessageLX200(SetLX200(cmd), false); }
        }
      break;
      case 3:
        menuSetFocBacklash(foc);
      break;
      case 4:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_FOC_TC, &isOk)) {
          sprintf(cmd, ":FA%u#", foc);
          if (isOk) {
            if (DisplayMessageLX200(SetLX200(cmd))) DisplayMessageLX200(SetLX200(":Fc1#"), false);
          } else {
            if (DisplayMessageLX200(SetLX200(cmd))) DisplayMessageLX200(SetLX200(":Fc0#"), false);
          }
        }
      break;
      case 5:
        menuSetFocTCCoef(foc);
      break;
      case 6:
        menuSetFocTCDeadband(foc);
      break;
    }
  }
}

bool UI::menuSetFocBacklash(uint8_t &foc) {
  float backlash;
  if (!DisplayMessageLX200(readFocBacklashLX200(foc, backlash))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_BACKLASH, foc);
  else
    sprintf(text, "Focuser " L_FOC_BACKLASH);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "", &backlash, 0, 999, 3, 0, " " L_FOC_BL_UNITS))
  {
    return DisplayMessageLX200(writeFocBacklashLX200(foc, backlash), false);
  }
  return true;
}

bool UI::menuSetFocTCCoef(uint8_t &foc) {
  float tccoef;
  if (!DisplayMessageLX200(readFocTCCoefLX200(foc, tccoef))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_TC_COEF, foc);
  else
    sprintf(text, "Focuser " L_FOC_TC_COEF);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "", &tccoef, -999, 999, 4, 0, " " L_MICRON_PER_C)) {
    return DisplayMessageLX200(writeFocTCCoefLX200(foc, tccoef), false);
  }
  return true;
}

bool UI::menuSetFocTCDeadband(uint8_t &foc) {
  float deadband;
  if (!DisplayMessageLX200(readFocTCDeadbandLX200(foc, deadband))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_TC_DEADBAND, foc);
  else
    sprintf(text, "Focuser " L_FOC_TC_DEADBAND);
  if (display->UserInterfaceInputValueFloat(&buttonPad, text, "", &deadband, 1, 999, 3, 0, " " L_FOC_TC_DB_UNITS)) {
    return DisplayMessageLX200(writeFocTCDeadbandLX200(foc, deadband), false);
  }
  return true;
}

void UI::menuRotator() {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    char string_list_SiteL2[80];
    if (status.hasDeRotator())
      strcpy(string_list_SiteL2,L_ROT_RET_HOME "\n" L_ROT_AT_HOME "\n" L_ROT_DEROT "\n" L_ROT_PA "\n" L_ROT_REV);
    else
      strcpy(string_list_SiteL2,L_ROT_RET_HOME "\n" L_ROT_AT_HOME);
    current_selection_L2 = display->UserInterfaceSelectionList(&buttonPad, L_ROTATOR, current_selection_L2, string_list_SiteL2);
    bool isOk = false;
    switch (current_selection_L2) {
      case 1: SetLX200(":rC#"); DisplayMessage(L_VALUE, L_SETV "!", 1500); break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_ROT_AT_HOME_ZERO "?", &isOk)) {
          if (isOk) { SetLX200(":rF#"); DisplayMessage(L_VALUE, L_SETV "!", 1500); }
        }
      break;
      case 3:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_ROT_DEROT " " L_ON "?", &isOk)) {
          if (isOk) { SetLX200(":r+#"); DisplayMessage(L_VALUE, L_SETV "!", 1500); } else { SetLX200(":r-#"); DisplayMessage(L_VALUE, L_SETV "!", 1500); }
        }
      break;
      case 4: DisplayMessageLX200(SetLX200(":rP#"),true); break;
      case 5:
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_ROT_REVERSE "?", &isOk)) {
          if (isOk) { SetLX200(":rR#"); DisplayMessage(L_VALUE, L_SETV "!", 1500); }
        }
      break;
    }
  }
}

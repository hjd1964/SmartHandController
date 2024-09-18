// -----------------------------------------------------------------------------------
// MenuSettings, for UserInterface
#include "../UserInterface.h"
extern NVS nv;

#include "../../lib/convert/Convert.h"

void UI::menuSettings() {
  current_selection_L1 = 1;
  while (current_selection_L1 != 0) {
    char string_list_SettingsL1[150] = "";

    int i = 1;
    int index[23] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
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
        index[9 + n] = i++;
        strcat(string_list_SettingsL1,"\n" L_SET_FOCUSER);
        nums[0] = '1' + n;
        if (status.getFocuserCount() > 1) strcat(string_list_SettingsL1, nums);
      }
    }

    if (status.featureFound()) {
      for (int n = 0; n < 8; n++) {
        status.featureSelect(n + 1);
        if (status.featurePurpose() == DEW_HEATER || status.featurePurpose() == INTERVALOMETER) {
          index[15 + n] = i++;
          strcat(string_list_SettingsL1, "\n");
          strcat(string_list_SettingsL1, status.featureName());
        }
      }
    }

    current_selection_L1 = display->UserInterfaceSelectionList(&keyPad, L_SETTINGS, current_selection_L1, string_list_SettingsL1);
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
    if (current_selection_L1 == index[15]) menuFeature(1);
    if (current_selection_L1 == index[16]) menuFeature(2);
    if (current_selection_L1 == index[17]) menuFeature(3);
    if (current_selection_L1 == index[18]) menuFeature(4);
    if (current_selection_L1 == index[19]) menuFeature(5);
    if (current_selection_L1 == index[20]) menuFeature(6);
    if (current_selection_L1 == index[21]) menuFeature(7);
    if (current_selection_L1 == index[22]) menuFeature(8);
  }
}

void UI::menuLocalDateTime() {
  char out[20];
  // Date
  if (message.show(onStep.Get(":GC#", out))) {
    char* pEnd;
    uint8_t month = strtol(&out[0], &pEnd, 10);
    uint8_t day = strtol(&out[3], &pEnd, 10);
    uint8_t year = strtol(&out[6], &pEnd, 10);
    if (display->UserInterfaceInputValueDate(&keyPad, L_SET_LOCAL_DATE, year, month, day)) {
      sprintf(out, ":SC%02d/%02d/%02d#", month, day, year);
      message.show(onStep.Set(out), false);
      // Time
      long value;
      boolean pmf = false;
      boolean dst = false;
      if (message.show(onStep.GetTime(value))) {
        if ((!hrs24) && (value >= 43200)) {
          value -= 43200;
          pmf = true;
        }
        if (display->UserInterfaceInputValueTime(&keyPad, &value, hrs24)) {
          if (hrs24 || (display->UserInterfaceInputValueBoolean(&keyPad, L_SET_LOCAL_PM "?", &pmf))) {
            if (pmf) value += 43200; // AM or PM?
            if (display->UserInterfaceInputValueBoolean(&keyPad, L_SET_LOCAL_DST "?", &dst)) {
              if (dst) value -= 3600; // Dst?
              message.show(onStep.SetTime(value),false);
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
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, L_DISPLAY, current_selection_L2, string_list_Display);
    switch (current_selection_L2) {
      case 1:
        message.show(L_SET_DISP_MSG1, L_SET_DISP_MSG2, 1500);
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
  current_selection_L3 = displaySettings.maxContrastSelection + 1;
  
  current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_SET_DISP_CONTRAST, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    displaySettings.maxContrastSelection = current_selection_L3 - 1;
    display->setContrast(UI::Contrast[displaySettings.maxContrastSelection]);
    nv.writeBytes(NV_DISPLAY_SETTINGS_BASE, &displaySettings, sizeof(DisplaySettings));
  }
}

void UI::menuDimTimeout() {
  const char *string_list_Display = L_DISABLE "\n""30 " L_SEC_ABV "\n""60 " L_SEC_ABV;
  current_selection_L3 = displaySettings.dimTimeoutSelection + 1;

  current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_SET_DISP_DIM_TO, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    displaySettings.dimTimeoutSelection = current_selection_L3 - 1;
    displaySettings.dimTimeout = displaySettings.dimTimeoutSelection * 30000;
    nv.writeBytes(NV_DISPLAY_SETTINGS_BASE, &displaySettings, sizeof(DisplaySettings));
  }
}

void UI::menuBlankTimeout() {
  const char *string_list_Display = L_DISABLE "\n""1 " L_MIN_ABV "\n""2 " L_MIN_ABV "\n""3 " L_MIN_ABV "\n""4 " L_MIN_ABV "\n""5 " L_MIN_ABV;
  current_selection_L3 = displaySettings.blankTimeoutSelection + 1;

  current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_SET_DISP_BLANK_TO, current_selection_L3, string_list_Display);
  if (current_selection_L3 > 0) {
    displaySettings.blankTimeoutSelection = current_selection_L3 - 1;
    displaySettings.blankTimeout = displaySettings.blankTimeoutSelection * 60 * 1000;
    nv.writeBytes(NV_DISPLAY_SETTINGS_BASE, &displaySettings, sizeof(DisplaySettings));
  }
}

void UI::menuSound() {
  boolean sound = false;
  if (display->UserInterfaceInputValueBoolean(&keyPad, L_SET_BUZZER, &sound)) {
    if (sound) message.show(onStep.Set(":SX97,1#"),false); else message.show(onStep.Set(":SX97,0#"),false);
  }
}

void UI::menuMeridianFlips() {
  const char *string_list = L_NOW "!" "\n" L_SET_MF_AUTO "\n" L_SET_MF_PAUSE;
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    boolean autoflip = false;
    boolean pause = false;
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, L_SET_MF, current_selection_L2, string_list);
    switch (current_selection_L2) {
      case 1:
        message.show(onStep.Set(":MN#"),false);
      break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_SET_MF_AF, &autoflip)) {
          if (autoflip) message.show(onStep.Set(":SX95,1#"),false); else message.show(onStep.Set(":SX95,0#"),false);
        }
      break;
      case 3:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_SET_MF_PF, &pause)) {
          if (pause) message.show(onStep.Set(":SX97,1#"),false); else message.show(onStep.Set(":SX97,0#"),false);
        }
      break;
    }
  }
}

void UI::menuSite() {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    const char *string_list_SiteL2 = L_SET_SITE_SELECT "\n" L_SET_SITE_LAT "\n" L_SET_SITE_LONG "\n" L_SET_SITE_UTC;
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, L_SET_SITE_TITLE, current_selection_L2, string_list_SiteL2);
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
  if (message.show(onStep.GetSite(val))) {
    current_selection_L3 = val;
    const char *string_list_SiteL3 = L_SET_SITE_NUM " 1" "\n" L_SET_SITE_NUM " 2" "\n" L_SET_SITE_NUM " 3" "\n" L_SET_SITE_NUM " 4";
    current_selection_L3 = display->UserInterfaceSelectionList(&keyPad, L_SET_SITE_NUM_TITLE, current_selection_L3, string_list_SiteL3);
    if (current_selection_L3 != 0) {
      val = current_selection_L3 - 1;
      onStep.SetSite(val);
    }
  }
}

void UI::menuLatitude() {
  char out[20];
  if (message.show(onStep.Get(":Gt#", out))) {
    char* pEnd;
    int degree = (int)strtol(&out[1], &pEnd, 10);
    int minute = (int)strtol(&out[4], &pEnd, 10);
    long angle = degree * 60 + minute;
    if (out[0] == '-') angle = -angle;
    angle *= 60;
    if (display->UserInterfaceInputValueLatitude(&keyPad, &angle)) {
      angle = angle / 60;
      minute = abs(angle) % 60;
      degree = abs(angle) / 60;
      sprintf(out, ":St%c%02d*%02d#", angle < 0 ? '-' : '+', degree, minute);
      message.show(onStep.Set(out),false);
    }
  }
}

void UI::menuLongitude() {
  char out[20];
  if (message.show(onStep.Get(":Gg#", out))) {
    char* pEnd;
    int degree = (int)strtol(&out[1], &pEnd, 10);
    int minute = (int)strtol(&out[5], &pEnd, 10);
    long angle = degree * 60 + minute;
    if (out[0] == '-') angle = -angle;
    angle *= 60;    
    if (display->UserInterfaceInputValueLongitude(&keyPad, &angle)) {
      angle = angle / 60;
      minute = abs(angle) % 60;
      degree = abs(angle) / 60;
      sprintf(out, ":Sg%c%03d*%02d#", angle < 0 ? '-' : '+', degree, minute);
      message.show(onStep.Set(out), false);
    }
  }
}

void UI::menuZone() {
  char out[20];
  if (message.show(onStep.Get(":GG#", out))) {
    char* pEnd;
    int hr = (int)strtol(&out[0], &pEnd, 10);

    boolean negative=false;
    if (hr<0) negative=true;
    uint8_t b=abs(hr);
  
    if (display->UserInterfaceInputValueInteger(&keyPad, "UTC " L_OFFSET_ABV " (-" L_TZ_ABV ")", "", &b, 0, 14, 2, " " L_HRS_ABV)) {
      if (display->UserInterfaceInputValueBoolean(&keyPad, "UTC " L_OFFSET_ABV " - ?", &negative)) {
        hr = b;
        if (negative) hr = -hr;
        sprintf(out, ":SG%+02d#", hr);
        message.show(onStep.Set(out), false);
      }
    }
  }
}

void UI::menuFirmware() {
  char out[64];
  
  sprintf(out, "SHC %s", _version);
  message.show(out, __DATE__, 3000);

  char temp1[32];
  char temp2[32];
  if ( (message.show(onStep.Get(":GVN#", temp1)))&&(message.show(onStep.Get(":GVD#", temp2))) )
  { for (char* p = temp1; (p = strchr(p, '#')); ++p) { *p = 0;} 
    for (char* p = temp2; (p = strchr(p, '#')); ++p) { *p = 0;} 
    sprintf(out,"OnStep %s",temp1);
    message.show(out, temp2, 3000);
  }
}

void UI::menuFocuser(uint8_t foc) {
  current_selection_L2 = 1;
  while (current_selection_L2 != 0) {
    const char *string_list_SiteL2 = L_FOC_RET_HOME "\n" L_FOC_AT_HOME "\n" L_FOC_BACKLASH "\n" L_FOC_TC "\n" L_FOC_TC_COEF "\n" L_FOC_TC_DEADBAND;
    
    char title[40];
    if (status.getFocuserCount() > 1) sprintf(title, "%s %i", L_FOCUSER, foc); else sprintf(title, "%s", L_FOCUSER);
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, title, current_selection_L2, string_list_SiteL2);

    bool isOk = false;
    char cmd[40];
    switch (current_selection_L2) {
      case 1:
        sprintf(cmd, ":FA%u#:Fh#", foc);
        message.show(onStep.Set(cmd),false);
      break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_FOC_AT_HALF, &isOk)) {
          sprintf(cmd, ":FA%u#:FH#", foc);
          if (isOk) { message.show(onStep.Set(cmd), false); }
        }
      break;
      case 3:
        menuSetFocBacklash(foc);
      break;
      case 4:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_FOC_TC, &isOk)) {
          sprintf(cmd, ":FA%u#", foc);
          if (isOk) {
            if (message.show(onStep.Set(cmd))) message.show(onStep.Set(":Fc1#"), false);
          } else {
            if (message.show(onStep.Set(cmd))) message.show(onStep.Set(":Fc0#"), false);
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
  if (!message.show(onStep.readFocBacklash(foc, backlash))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_BACKLASH, foc);
  else
    sprintf(text, "Focuser " L_FOC_BACKLASH);
  if (display->UserInterfaceInputValueFloat(&keyPad, text, "", &backlash, 0, 999, 3, 0, " " L_FOC_BL_UNITS))
  {
    return message.show(onStep.writeFocBacklash(foc, backlash), false);
  }
  return true;
}

bool UI::menuSetFocTCCoef(uint8_t &foc) {
  float tccoef;
  if (!message.show(onStep.readFocTCCoef(foc, tccoef))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_TC_COEF, foc);
  else
    sprintf(text, "Focuser " L_FOC_TC_COEF);
  if (display->UserInterfaceInputValueFloat(&keyPad, text, "", &tccoef, -999, 999, 4, 0, " " L_MICRON_PER_C)) {
    return message.show(onStep.writeFocTCCoef(foc, tccoef), false);
  }
  return true;
}

bool UI::menuSetFocTCDeadband(uint8_t &foc) {
  float deadband;
  if (!message.show(onStep.readFocTCDeadband(foc, deadband))) return false;
  char text[20];
  if (status.getFocuserCount() > 1)
    sprintf(text, "Foc.%u " L_FOC_TC_DEADBAND, foc);
  else
    sprintf(text, "Focuser " L_FOC_TC_DEADBAND);
  if (display->UserInterfaceInputValueFloat(&keyPad, text, "", &deadband, 1, 999, 3, 0, " " L_FOC_TC_DB_UNITS)) {
    return message.show(onStep.writeFocTCDeadband(foc, deadband), false);
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
    current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, L_ROTATOR, current_selection_L2, string_list_SiteL2);
    bool isOk = false;
    switch (current_selection_L2) {
      case 1: onStep.Set(":rC#"); message.show(L_VALUE, L_SETV "!", 1500); break;
      case 2:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_ROT_AT_HOME_ZERO "?", &isOk)) {
          if (isOk) { onStep.Set(":rF#"); message.show(L_VALUE, L_SETV "!", 1500); }
        }
      break;
      case 3:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_ROT_DEROT " " L_ON "?", &isOk)) {
          if (isOk) { onStep.Set(":r+#"); message.show(L_VALUE, L_SETV "!", 1500); } else { onStep.Set(":r-#"); message.show(L_VALUE, L_SETV "!", 1500); }
        }
      break;
      case 4: message.show(onStep.Set(":rP#"),true); break;
      case 5:
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_ROT_REVERSE "?", &isOk)) {
          if (isOk) { onStep.Set(":rR#"); message.show(L_VALUE, L_SETV "!", 1500); }
        }
      break;
    }
  }
}

void UI::menuFeature(uint8_t feature) {
  status.featureSelect(feature);

  if (status.featurePurpose() == DEW_HEATER) {
    current_selection_L2 = 1;
    while (current_selection_L2 != 0) {
      status.featureUpdate(feature);

      const char *string_list_SiteL2 = L_AF_DEW_HEATER_ZERO "\n" L_AF_DEW_HEATER_SPAN;
      
      char title[40];
      sprintf(title, "%s", status.featureName());
      current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, title, current_selection_L2, string_list_SiteL2);

      char cmd[64];
      char cmdParam[40];
      int low = -5;
      int high = 20;
      float value;
      switch (current_selection_L2) {
        case 1:
          value = celsiusToNativeRelative(status.featureValue2());
          if (display->UserInterfaceInputValueFloat(&keyPad, L_AF_DEW_HEATER_ZERO, "", &value, celsiusToNativeRelative(low), celsiusToNativeRelative(high), 4, 1, " " TEMPERATURE_UNITS_ABV)) {
            sprintF(cmdParam, "%0.1f", nativeToCelsiusRelative(value));
            sprintf(cmd, ":SXX%i,Z%s#", status.featureNumber(), cmdParam);
            message.show(onStep.Set(cmd), false);
          }
        break;
        case 2:
          value = celsiusToNativeRelative(status.featureValue3());
          if (display->UserInterfaceInputValueFloat(&keyPad, L_AF_DEW_HEATER_SPAN, "", &value, celsiusToNativeRelative(low), celsiusToNativeRelative(high), 4, 1, " " TEMPERATURE_UNITS_ABV)) {
            sprintF(cmdParam, "%0.1f", nativeToCelsiusRelative(value));
            sprintf(cmd, ":SXX%i,S%s#", status.featureNumber(), cmdParam);
            message.show(onStep.Set(cmd), false);
          }
        break;
      }
    }
  }

  if (status.featurePurpose() == INTERVALOMETER) {
    current_selection_L2 = 1;
    while (current_selection_L2 != 0) {
      status.featureUpdate(feature);

      const char *string_list_SiteL2 = L_AF_IV_COUNT "\n" L_AF_IV_DELAY "\n" L_AF_IV_EXPOS;

      char title[40];
      sprintf(title, "%s", status.featureName());
      current_selection_L2 = display->UserInterfaceSelectionList(&keyPad, title, current_selection_L2, string_list_SiteL2);

      char cmd[40];
      float value;
      switch (current_selection_L2) {
        case 1:
          value = status.featureValue4();
          if (display->UserInterfaceInputValueFloat(&keyPad, L_AF_IV_COUNT, "", &value, 0, 255, 3, 0, "")) {
            sprintf(cmd, ":SXX%i,C%i#", status.featureNumber(), (int)lround(value));
            message.show(onStep.Set(cmd), false);
          }
        break;
        case 2:
          value = status.featureValue3();
          if (display->UserInterfaceInputValueFloat(&keyPad, L_AF_IV_DELAY, "", &value, 0, 3600, 4, 0, " secs")) {
            sprintf(cmd, ":SXX%i,D%i#", status.featureNumber(), (int)lround(value));
            message.show(onStep.Set(cmd), false);
          }
        break;
        case 3:
          value = status.featureValue2();
          if (display->UserInterfaceInputValueFloat(&keyPad, L_AF_IV_EXPOS, "", &value, 0, 3600, 4, 0, " secs")) {
            sprintf(cmd, ":SXX%i,E%i#", status.featureNumber(), (int)lround(value));
            message.show(onStep.Set(cmd), false);
          }
        break;
      }
    }
  }
}

// -----------------------------------------------------------------------------------
// MenuMain, for UserInterface
#include "../UserInterface.h"

void UI::menuFeatureKey() {
  static unsigned short current_selection_feature_mode = 1;
  unsigned short last_selection_feature_mode = current_selection_feature_mode;

  char string_feature_Modes[120] = L_FKEY_GUIDE_RATE "\n" L_FKEY_PULSE_GUIDE_RATE;

  int i = 2;
  int j[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
  #if UTILITY_LIGHT != OFF
    { i++; j[0]=i; strcat(string_feature_Modes,"\n" L_FKEY_UTILITY_LIGHT); }
  #endif
  if (status.hasReticle())  { i++; j[1] = i; strcat(string_feature_Modes,"\n" L_FKEY_RETICLE);  }
  if (status.hasRotator())  { i++; j[2] = i; strcat(string_feature_Modes,"\n" L_FKEY_ROTATOR);  }
  for (int n = 0; n < 6; n++) {
    char nums[] = "0";
    if (status.hasFocuser(n)) {
      i++;
      j[n + 3] = i;
      strcat(string_feature_Modes,"\n" L_FKEY_FOCUSER);
      nums[0] = '1' + n;
      if (status.getFocuserCount() > 1) { strcat(string_feature_Modes, " "); strcat(string_feature_Modes, nums); }
    }
  }

  current_selection_feature_mode = display->UserInterfaceSelectionList(&buttonPad, L_FKEY_FEATURE_KEYS, current_selection_feature_mode, string_feature_Modes);

  if (last_selection_feature_mode > 0) {
    if (current_selection_feature_mode == 1) featureKeyMode = 1; else // guide rate
    if (current_selection_feature_mode == 2) featureKeyMode = 2; else // pulse guide rate
    if (current_selection_feature_mode == j[0]) featureKeyMode = 3; else // util. light
    if (current_selection_feature_mode == j[1]) featureKeyMode = 4; else // reticule
    if (current_selection_feature_mode == j[2]) featureKeyMode = 5; else // rotator
    if (current_selection_feature_mode == j[3]) { featureKeyMode = 6; SetLX200(":FA1#"); } else // focuser 1
    if (current_selection_feature_mode == j[4]) { featureKeyMode = 7; SetLX200(":FA2#"); } else // focuser 2
    if (current_selection_feature_mode == j[5]) { featureKeyMode = 8; SetLX200(":FA3#"); } else // focuser 3
    if (current_selection_feature_mode == j[6]) { featureKeyMode = 9; SetLX200(":FA4#"); } else // focuser 4
    if (current_selection_feature_mode == j[7]) { featureKeyMode = 10; SetLX200(":FA5#"); } else // focuser 5
    if (current_selection_feature_mode == j[8]) { featureKeyMode = 11; SetLX200(":FA6#"); } else // focuser 6
    { featureKeyMode = 1; current_selection_feature_mode = 1; } // default to guide rate
  } else current_selection_feature_mode = last_selection_feature_mode;
}

// Main Menu
void UI::menuMain() {
  current_selection_L0 = 1;
  while (current_selection_L0 != 0) {
    if (status.isPecEnabled()) {
      const char *string_list_main_UnParkedL0 = L_MM_GOTO "\n" L_MM_SYNC "\n" L_MM_ALIGN "\n" L_MM_PARKING "\n" L_MM_TRACKING "\n" L_MM_PEC "\n" L_MM_SETTINGS;
      current_selection_L0 = display->UserInterfaceSelectionList(&buttonPad, L_MM_MAIN_MENU, current_selection_L0, string_list_main_UnParkedL0);
      switch (current_selection_L0) {
        case 1: if (menuSyncGoto(false) == MR_QUIT) return; break;
        case 2: if (menuSyncGoto(true) == MR_QUIT) return; break;
        case 3: menuAlignment(); break;
        case 4: menuParking(); break;
        case 5: menuTracking(); break;
        case 6: menuPEC(); break;
        case 7: menuSettings(); break;
      }
    } else {
      const char *string_list_main_UnParkedL0 = L_MM_GOTO "\n" L_MM_SYNC "\n" L_MM_ALIGN "\n" L_MM_PARKING "\n" L_MM_TRACKING "\n" L_MM_SETTINGS;
      current_selection_L0 = display->UserInterfaceSelectionList(&buttonPad, L_MM_MAIN_MENU, current_selection_L0, string_list_main_UnParkedL0);
      switch (current_selection_L0) {
        case 1: if (menuSyncGoto(false) == MR_QUIT) return; break;
        case 2: if (menuSyncGoto(true) == MR_QUIT) return; break;
        case 3: menuAlignment(); break;
        case 4: menuParking(); break;
        case 5: menuTracking(); break;
        case 6: menuSettings(); break;
      }
    }
  }
}

// Parking Menu
void UI::menuParking() {
  Status::ParkState currentstate = status.getParkState();
  if (currentstate == Status::PRK_PARKED) { }
  if (currentstate == Status::PRK_UNPARKED) { }

  current_selection_L1 = 1;
  while (current_selection_L1 != 0) {
    const char *string_list_SettingsL1 = L_PARK "\n" L_UNPARK "\n" L_SETPARK;
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, L_PARKING, current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1) {
      case 1:
        if (SetLX200(":hP#")== LX200VALUESET) {
          DisplayMessage(L_PARKING, L_TELESCOPE, 500); 
          current_selection_L1 = 0;
          current_selection_L0 = 0;
        } else DisplayMessage(L_PARK, L_FAILED, 1000);
      break;
      case 2:
        if (SetLX200(":hR#")== LX200VALUESET) {
          DisplayMessage(L_UNPARKING, L_TELESCOPE, 500); 
          current_selection_L1 = 0;
        } else DisplayMessage(L_UNPARK, L_FAILED, 1000);
      break;
      case 3: 
        boolean SetP=false; 
        if (display->UserInterfaceInputValueBoolean(&buttonPad, L_SETPARK "?", &SetP)) {
          if (SetP) {
            if (SetLX200(":hQ#")== LX200VALUESET) {
              DisplayMessage(L_SETPARK, L_OK, 500);
              current_selection_L1 = 0;
            } else DisplayMessage(L_SETPARK, L_FAILED, 1000); 
          } else DisplayMessage(L_SETPARK, L_CANCELED, 500);
        } else DisplayMessage(L_SETPARK, L_CANCELED, 500);
      break;
    }
  }
}

void UI::menuTracking() {
  Status::TrackState currentstate = status.getTrackingState();

  if (status.isMountAltAz()) {
    const char *string_list;
    current_selection_L1 = 1;
    while (current_selection_L1 != 0) {
      if (currentstate == Status::TRK_ON) {
        string_list = L_TRK_STOP "\n" L_TRK_SIDEREAL "\n" L_TRK_SOLAR "\n" L_TRK_LUNAR "\n" L_TRK_RESET "\n" L_TRK_FASTER "\n" L_TRK_SLOWER;
      } else {
        string_list = L_TRK_START "\n" L_TRK_SIDEREAL "\n" L_TRK_SOLAR "\n" L_TRK_LUNAR "\n" L_TRK_RESET "\n" L_TRK_FASTER "\n" L_TRK_SLOWER;
      }
      current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, L_TRACKING, current_selection_L1, string_list);
      switch (current_selection_L1) {
        case 1:
          if (currentstate == Status::TRK_ON) {
            if (SetLX200(":Td#")== LX200VALUESET) { DisplayMessage(L_TRACKING, L_OFF, 500); currentstate=Status::TRK_OFF; } else DisplayMessage(L_SET_STATE, L_FAILED, 1000);
          } else {
            if (SetLX200(":Te#")== LX200VALUESET) { DisplayMessage(L_TRACKING, L_ON, 500); currentstate=Status::TRK_ON; } else DisplayMessage(L_SET_STATE, L_FAILED, 1000);
          }
        break;
        case 2: DisplayMessageLX200(SetLX200(":TQ#"), false); break;
        case 3: DisplayMessageLX200(SetLX200(":TS#"), false); break;
        case 4: DisplayMessageLX200(SetLX200(":TL#"), false); break;
        case 5: DisplayMessageLX200(SetLX200(":TR#"), false); break;
        case 6: DisplayMessageLX200(SetLX200(":T+#"), false); break;
        case 7: DisplayMessageLX200(SetLX200(":T-#"), false); break;
      }
    }
  } else {
    const char *string_list;
    current_selection_L1 = 1;
    while (current_selection_L1 != 0) {
      if (currentstate == Status::TRK_ON) {
        string_list = L_TRK_STOP "\n" L_TRK_SIDEREAL "\n" L_TRK_SOLAR "\n" L_TRK_LUNAR "\n" L_TRK_CF "\n" L_TRK_CR "\n" L_TRK_CO "\n" L_TRK_CS "\n" L_TRK_CD "\n" L_TRK_RESET "\n" L_TRK_FASTER "\n" L_TRK_SLOWER;
      } else {
        string_list = L_TRK_START "\n" L_TRK_SIDEREAL "\n" L_TRK_SOLAR "\n" L_TRK_LUNAR "\n" L_TRK_CF "\n" L_TRK_CR "\n" L_TRK_CO "\n" L_TRK_CS "\n" L_TRK_CD "\n" L_TRK_RESET "\n" L_TRK_FASTER "\n" L_TRK_SLOWER;
      }
      current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, L_TRACKING, current_selection_L1, string_list);
      switch (current_selection_L1) {
        case 1:
          if (currentstate == Status::TRK_ON) {
            if (SetLX200(":Td#")== LX200VALUESET) { DisplayMessage(L_TRACKING, L_OFF, 500); currentstate=Status::TRK_OFF; } else DisplayMessage(L_SET_STATE, L_FAILED, 1000);
          } else {
            if (SetLX200(":Te#")== LX200VALUESET) { DisplayMessage(L_TRACKING, L_ON, 500); currentstate=Status::TRK_ON; } else DisplayMessage(L_SET_STATE, L_FAILED, 1000);
          }
        break;
        case 2:  DisplayMessageLX200(SetLX200(":TQ#"), false); break;
        case 3:  DisplayMessageLX200(SetLX200(":TS#"), false); break;
        case 4:  DisplayMessageLX200(SetLX200(":TL#"), false); break;
        case 5:  DisplayMessageLX200(SetLX200(":To#"), false); break;
        case 6:  DisplayMessageLX200(SetLX200(":Tr#"), false); break;
        case 7:  DisplayMessageLX200(SetLX200(":Tn#"), false); break;
        case 8:  DisplayMessageLX200(SetLX200(":T1#"), false); break;
        case 9:  DisplayMessageLX200(SetLX200(":T2#"), false); break;
        case 10: DisplayMessageLX200(SetLX200(":TR#"), false); break;
        case 11: DisplayMessageLX200(SetLX200(":T+#"), false); break;
        case 12: DisplayMessageLX200(SetLX200(":T-#"), false); break;
      }
    }
  }
}

// PEC Menu
void UI::menuPEC() {
  current_selection_L1 = 1;
  while (current_selection_L1 != 0)
  {
    const char *string_list_SettingsL1 = L_PEC_PLAY "\n" L_PEC_STOP "\n" L_PEC_CLEAR "\n" L_PEC_RECORD "\n" L_PEC_WRITENV;
    current_selection_L1 = display->UserInterfaceSelectionList(&buttonPad, L_PEC, current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1)
    {
    case 1: DisplayMessageLX200(SetLX200(":$QZ+#"), true); current_selection_L0 = 0; current_selection_L1 = 0; DisplayMessage(L_PEC, L_PEC_PLAYING, 1000); break;
    case 2: DisplayMessageLX200(SetLX200(":$QZ-#"), true); current_selection_L0 = 0; current_selection_L1 = 0; DisplayMessage(L_PEC, L_PEC_STOPPED, 1000); break;
    case 3: DisplayMessageLX200(SetLX200(":$QZZ#"), false); break;
    case 4: DisplayMessageLX200(SetLX200(":$QZ/#"), true); current_selection_L0 = 0; current_selection_L1 = 0; DisplayMessage(L_PEC, L_PEC_RECORDING, 1000); break;
    case 5: DisplayMessageLX200(SetLX200(":$QZ!#"), false); break;
    default: break;
    }
  }
}

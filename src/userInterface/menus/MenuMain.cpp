// -----------------------------------------------------------------------------------
// MenuMain, for UserInterface
#include "../UserInterface.h"

#if SERIAL_IP_MODE != OFF || SERIAL_BT_MODE != OFF

  #ifndef SERIAL_BT_DISCOVERY_TIME
    #define SERIAL_BT_DISCOVERY_TIME 5000
  #endif

  bool UI::menuWireless() {
  rescan:
    display->clearDisplay();
    display->firstPage(); do {
      display->drawStr(33, 25, "Scanning");
      display->drawStr(22, 50, "Please wait...");
    } while ( display->nextPage() );

    unsigned short current_selection_connect;
    current_selection_connect = 1;

    int selectionCount;
    selectionCount = 0;
    int ssidMatchCount = 0;
    UNUSED(ssidMatchCount);

    char selection_list[240];
    selection_list[0] = 0;

    // build the menu selections
    selectionCount++;
    strcat(selection_list, L_REFRESH "\n");

    // first serial if it's available
    #if SERIAL_ONSTEP != OFF
      selectionCount++;
      strcat(selection_list, "Serial\n");
    #endif

    #if SERIAL_IP_MODE != OFF
      int ssid_cross_index[3];
      int ssidCount = WiFi.scanNetworks();
      VF("MSG: Connect menu, found "); V(ssidCount); VLF(" WiFi networks");

      // then any matching SSID's for active SHC WiFi stations
      for (int i = 0; i < 3; i++) {
        for (int ssid = 0; ssid < ssidCount; ssid++) {
          if ((strlen(wifiManager.settings.station[i].host) > 0) &&
              (WiFi.SSID(ssid).equals(wifiManager.settings.station[i].ssid))) {
            VF("MSG: Connect menu, added "); V(wifiManager.settings.station[i].host);
            VF(" w/SSID "); VL(wifiManager.settings.station[i].ssid);

            if (strlen(selection_list) + 18 < sizeof(selection_list)) {
              selectionCount++;
              strncat(selection_list, wifiManager.settings.station[i].host, 14);
              strcat(selection_list, " ip\n");
              ssid_cross_index[ssidMatchCount++] = i + 1;
            }
            break;
          }
        }
      }
    #endif

    #if SERIAL_BT_MODE != OFF
      VF("MSG: Connect menu, starting asynchronous BT discovery");

      BTScanResults *btDeviceList = SERIAL_BT.getScanResults();  // maybe accessing from different threads!
      if (SERIAL_BT.discoverAsync([](BTAdvertisedDevice *pDevice) { VF("."); })) {
        delay(SERIAL_BT_DISCOVERY_TIME);
        VLF("");
        VLF("MSG: Connect menu, stopping asynchronous BT discovery");
        SERIAL_BT.discoverAsyncStop();
        delay(4000);

        VF("MSG: Connect menu, found "); V(btDeviceList->getCount()); VLF(" BT devices");

        if (btDeviceList->getCount() > 0) {
          for (int i = 0; i < btDeviceList->getCount(); i++) {
            BTAdvertisedDevice *device = btDeviceList->getDevice(i);
            if (strlen(device->getName().c_str()) != 0) {
              VF("MSG: Connect menu, device ");
              VF(device->getAddress().toString().c_str()); DF(" ");
              VLF(device->getName().c_str());
              // VLF(device->getRSSI()); // crashes why?

              std::map<int, std::string> channels = SERIAL_BT.getChannels(device->getAddress());

              VF("MSG: Connect menu, device found "); V(channels.size()); VLF(" services");
              for (auto const &entry: channels) {
                VF("MSG: Connect menu, device service "); V(entry.first); VF(" "); VL(entry.second.c_str());
              }

              if (strlen(selection_list) + 18 < sizeof(selection_list)) {
                selectionCount++;
                strncat(selection_list, device->getName().c_str(), 14);
                strcat(selection_list, " bt\n");
              }
            }
          }
        } else {
          VLF("MSG: Connect menu, no BT devices found");
        }
      } else {
        VLF("");
        VLF("MSG: Connect menu, discovery failed no BT devices found");
      }
    #endif

    selection_list[strlen(selection_list) - 1] = 0;

    // show the menu
    if (selectionCount > 0) {
      do {
        current_selection_connect = display->UserInterfaceSelectionList(&keyPad, L_WIFI_SELECT, current_selection_connect, selection_list);
      } while (current_selection_connect == 0);
    }

    VF("MSG: Connect menu, user selected ");

    // check for re-scan
    current_selection_connect--;
    if (current_selection_connect == 0) {
      VLF("Re-scan");
      goto rescan;
    }

    #if SERIAL_ONSTEP != OFF
      current_selection_connect--;
      if (current_selection_connect == 0) {
        onStep.connectionMode = CM_SERIAL;
        VLF("Serial");
      } else
    #endif

    #if SERIAL_IP_MODE != OFF
      if (current_selection_connect > 0 && current_selection_connect <= ssidMatchCount) {
        current_selection_connect--;

        // flag and start WiFi
        onStep.connectionMode = CM_WIFI;
        VF("WiFi "); VL(ssid_cross_index[current_selection_connect]);
        wifiManager.setStation(ssid_cross_index[current_selection_connect]);
      } else
    #endif

    #if SERIAL_BT_MODE != OFF
      if (btDeviceList->getCount() >= current_selection_connect - (ssidMatchCount + 1)) {
        current_selection_connect = current_selection_connect - (ssidMatchCount + 1);
        VF("Bluetooth "); VL(current_selection_connect);
        
        VF("MSG: Connect menu, scanning "); V(btDeviceList->getCount()); VLF(" BT devices");
        BTAddress addr;
        int channel = 0;

        BTAdvertisedDevice *device = btDeviceList->getDevice(current_selection_connect);
        VF("MSG: Connect menu, selected device ");
        VF(device->getAddress().toString().c_str()); DF(" ");
        VLF(device->getName().c_str());
        // VLF(device->getRSSI()); // crashes?

        std::map<int, std::string> channels = SERIAL_BT.getChannels(device->getAddress());

        if (channels.size() > 0) {
          addr = device->getAddress();
          channel = channels.begin()->first;
        }

        if (addr) {
          VF("MSG: Connect menu, connecting to "); V(addr.toString().c_str());
          VF(" channel "); V(channel); VF("...");
          if (SERIAL_BT.connect(addr, channel, ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE)) {
            VLF(" success");
            onStep.connectionMode = CM_BLUETOOTH;
          } else {
            VLF(" failed!");
            goto rescan;
          }
        }
      } else
    #endif

    {
      VLF("unknown");
    }

    return true;
  }
#endif

void UI::menuFeatureKey() {
  static unsigned short current_selection_feature_mode = 1;
  unsigned short last_selection_feature_mode = current_selection_feature_mode;

  char string_feature_Modes[240] = L_FKEY_GUIDE_RATE "\n" L_FKEY_PULSE_GUIDE_RATE;

  int i = 2;
  int j[17] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  #if UTILITY_LIGHT != OFF
    { i++; j[0] = i; strcat(string_feature_Modes,"\n" L_FKEY_UTILITY_LIGHT); }
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

  if (status.featureFound()) {
    for (int n = 0; n < 8; n++) {
      status.featureSelect(n + 1);
      if (status.featurePurpose() > 0) {
        i++;
        j[n + 9] = i;
        strcat(string_feature_Modes, "\n");
        strcat(string_feature_Modes, status.featureName());
      }
    }
  }

  current_selection_feature_mode = display->UserInterfaceSelectionList(&keyPad, L_FKEY_FEATURE_KEYS, current_selection_feature_mode, string_feature_Modes);

  if (last_selection_feature_mode > 0) {
    if (current_selection_feature_mode == 1) featureKeyMode = 1; else // guide rate
    if (current_selection_feature_mode == 2) featureKeyMode = 2; else // pulse guide rate
    if (current_selection_feature_mode == j[0]) featureKeyMode = 3; else // util. light
    if (current_selection_feature_mode == j[1]) featureKeyMode = 4; else // reticle
    if (current_selection_feature_mode == j[2]) featureKeyMode = 5; else // rotator
    if (current_selection_feature_mode == j[3]) { featureKeyMode = 6; onStepLx200.Set(":FA1#"); } else // focuser 1
    if (current_selection_feature_mode == j[4]) { featureKeyMode = 7; onStepLx200.Set(":FA2#"); } else // focuser 2
    if (current_selection_feature_mode == j[5]) { featureKeyMode = 8; onStepLx200.Set(":FA3#"); } else // focuser 3
    if (current_selection_feature_mode == j[6]) { featureKeyMode = 9; onStepLx200.Set(":FA4#"); } else // focuser 4
    if (current_selection_feature_mode == j[7]) { featureKeyMode = 10; onStepLx200.Set(":FA5#"); } else // focuser 5
    if (current_selection_feature_mode == j[8]) { featureKeyMode = 11; onStepLx200.Set(":FA6#"); } else // focuser 6
  	if (current_selection_feature_mode == j[9])  featureKeyMode = 12; else // feature 1
	  if (current_selection_feature_mode == j[10]) featureKeyMode = 13; else // feature 2
    if (current_selection_feature_mode == j[11]) featureKeyMode = 14; else // feature 3
    if (current_selection_feature_mode == j[12]) featureKeyMode = 15; else // feature 4
    if (current_selection_feature_mode == j[13]) featureKeyMode = 16; else // feature 5
    if (current_selection_feature_mode == j[14]) featureKeyMode = 17; else // feature 6
    if (current_selection_feature_mode == j[15]) featureKeyMode = 18; else // feature 7
    if (current_selection_feature_mode == j[16]) featureKeyMode = 19; else // feature 8
    { featureKeyMode = 1; current_selection_feature_mode = 1; } // default to guide rate
  } else current_selection_feature_mode = last_selection_feature_mode;
}

// Main Menu
void UI::menuMain() {
  current_selection_L0 = 1;
  while (current_selection_L0 != 0) {
    if (status.isPecEnabled()) {
      const char *string_list_main_UnParkedL0 = L_MM_GOTO "\n" L_MM_SYNC "\n" L_MM_ALIGN "\n" L_MM_PARKING "\n" L_MM_TRACKING "\n" L_MM_PEC "\n" L_MM_SETTINGS;
      current_selection_L0 = display->UserInterfaceSelectionList(&keyPad, L_MM_MAIN_MENU, current_selection_L0, string_list_main_UnParkedL0);
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
      current_selection_L0 = display->UserInterfaceSelectionList(&keyPad, L_MM_MAIN_MENU, current_selection_L0, string_list_main_UnParkedL0);
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
    current_selection_L1 = display->UserInterfaceSelectionList(&keyPad, L_PARKING, current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1) {
      case 1:
        if (onStepLx200.Set(":hP#")== CR_VALUE_SET) {
          message.show(L_PARKING, L_TELESCOPE, 500); 
          current_selection_L1 = 0;
          current_selection_L0 = 0;
        } else message.show(L_PARK, L_FAILED, 1000);
      break;
      case 2:
        if (onStepLx200.Set(":hR#")== CR_VALUE_SET) {
          message.show(L_UNPARKING, L_TELESCOPE, 500); 
          current_selection_L1 = 0;
        } else message.show(L_UNPARK, L_FAILED, 1000);
      break;
      case 3: 
        boolean SetP=false; 
        if (display->UserInterfaceInputValueBoolean(&keyPad, L_SETPARK "?", &SetP)) {
          if (SetP) {
            if (onStepLx200.Set(":hQ#")== CR_VALUE_SET) {
              message.show(L_SETPARK, L_OK, 500);
              current_selection_L1 = 0;
            } else message.show(L_SETPARK, L_FAILED, 1000); 
          } else message.show(L_SETPARK, L_CANCELED, 500);
        } else message.show(L_SETPARK, L_CANCELED, 500);
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
      current_selection_L1 = display->UserInterfaceSelectionList(&keyPad, L_TRACKING, current_selection_L1, string_list);
      switch (current_selection_L1) {
        case 1:
          if (currentstate == Status::TRK_ON) {
            if (onStepLx200.Set(":Td#")== CR_VALUE_SET) { message.show(L_TRACKING, L_OFF, 500); currentstate=Status::TRK_OFF; } else message.show(L_SET_STATE, L_FAILED, 1000);
          } else {
            if (onStepLx200.Set(":Te#")== CR_VALUE_SET) { message.show(L_TRACKING, L_ON, 500); currentstate=Status::TRK_ON; } else message.show(L_SET_STATE, L_FAILED, 1000);
          }
        break;
        case 2: message.show(onStepLx200.Set(":TQ#"), false); break;
        case 3: message.show(onStepLx200.Set(":TS#"), false); break;
        case 4: message.show(onStepLx200.Set(":TL#"), false); break;
        case 5: message.show(onStepLx200.Set(":TR#"), false); break;
        case 6: message.show(onStepLx200.Set(":T+#"), false); break;
        case 7: message.show(onStepLx200.Set(":T-#"), false); break;
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
      current_selection_L1 = display->UserInterfaceSelectionList(&keyPad, L_TRACKING, current_selection_L1, string_list);
      switch (current_selection_L1) {
        case 1:
          if (currentstate == Status::TRK_ON) {
            if (onStepLx200.Set(":Td#")== CR_VALUE_SET) { message.show(L_TRACKING, L_OFF, 500); currentstate=Status::TRK_OFF; } else message.show(L_SET_STATE, L_FAILED, 1000);
          } else {
            if (onStepLx200.Set(":Te#")== CR_VALUE_SET) { message.show(L_TRACKING, L_ON, 500); currentstate=Status::TRK_ON; } else message.show(L_SET_STATE, L_FAILED, 1000);
          }
        break;
        case 2:  message.show(onStepLx200.Set(":TQ#"), false); break;
        case 3:  message.show(onStepLx200.Set(":TS#"), false); break;
        case 4:  message.show(onStepLx200.Set(":TL#"), false); break;
        case 5:  message.show(onStepLx200.Set(":To#"), false); break;
        case 6:  message.show(onStepLx200.Set(":Tr#"), false); break;
        case 7:  message.show(onStepLx200.Set(":Tn#"), false); break;
        case 8:  message.show(onStepLx200.Set(":T1#"), false); break;
        case 9:  message.show(onStepLx200.Set(":T2#"), false); break;
        case 10: message.show(onStepLx200.Set(":TR#"), false); break;
        case 11: message.show(onStepLx200.Set(":T+#"), false); break;
        case 12: message.show(onStepLx200.Set(":T-#"), false); break;
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
    current_selection_L1 = display->UserInterfaceSelectionList(&keyPad, L_PEC, current_selection_L1, string_list_SettingsL1);
    switch (current_selection_L1)
    {
    case 1: message.show(onStepLx200.Set(":$QZ+#"), true); current_selection_L0 = 0; current_selection_L1 = 0; message.show(L_PEC, L_PEC_PLAYING, 1000); break;
    case 2: message.show(onStepLx200.Set(":$QZ-#"), true); current_selection_L0 = 0; current_selection_L1 = 0; message.show(L_PEC, L_PEC_STOPPED, 1000); break;
    case 3: message.show(onStepLx200.Set(":$QZZ#"), false); break;
    case 4: message.show(onStepLx200.Set(":$QZ/#"), true); current_selection_L0 = 0; current_selection_L1 = 0; message.show(L_PEC, L_PEC_RECORDING, 1000); break;
    case 5: message.show(onStepLx200.Set(":$QZ!#"), false); break;
    default: break;
    }
  }
}

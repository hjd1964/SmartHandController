// -----------------------------------------------------------------------------------
// MenuConnect, for UserInterface
#include "../UserInterface.h"
#include "../../lib/tasks/OnTask.h"

#if SERIAL_IP_MODE != OFF || SERIAL_BT_MODE != OFF

  #ifndef SERIAL_BT_DISCOVERY_TIME
    #define SERIAL_BT_DISCOVERY_TIME 6000
  #endif

  enum CrossIndexCode: uint8_t {CI_NONE, CI_IP_STATION_INDEX, CI_IP_LIST_INDEX, CI_BT_STATION_INDEX, CI_BT_LIST_INDEX};

  typedef struct CrossIndex {
    CrossIndexCode code;
    char mac[18];
    int index;
  } CrossIndex;

  bool UI::menuWireless() {
    char current_title[24];
    unsigned short userSelection;
    int selectionCount;
    char selectionList[480];

    const int crossIndexSize = 40;
    CrossIndex crossIndex[crossIndexSize];

    bool showEditList = false;
    int showAllSelection;
    int ssidCount = 0;
    UNUSED(ssidCount);
    bool scanDone = false;
    UNUSED(scanDone);

  rescan:
    // build the menu selections
    selectionCount = 0;
    selectionList[0] = 0;
    for (int i = 0; i < crossIndexSize; i++) crossIndex[i].code = CI_NONE;

    if (showEditList)
      strcpy(current_title, L_WIFI_SELECT_EDIT);
    else {
      strcpy(current_title, L_WIFI_SELECT);
      message.show(L_SCANNING_LINE1, L_SCANNING_LINE2, 10);
    }

    // first serial if it's available
    #if SERIAL_ONSTEP != OFF
      if (!showEditList) {
        selectionCount++;
        strcat(selectionList, "Serial\n");
      }
    #endif

    #if SERIAL_IP_MODE != OFF
      if (!showEditList) {
        ssidCount = WiFi.scanNetworks();
        VF("MSG: Connect menu, found "); V(ssidCount); VLF(" WiFi networks");
      }

      // then any matching SSID's for active SHC WiFi stations
      for (int ssidNumber = 0; ssidNumber < ssidCount; ssidNumber++) {
        if (strlen(selectionList) + 18 >= sizeof(selectionList)) break;

        VF("MSG: Connect menu, WiFi SSID "); V(WiFi.SSID(ssidNumber)); VF(" ");

        if (showEditList) {
          VLF("");
          if (++selectionCount > crossIndexSize) break;
          strncat(selectionList, WiFi.SSID(ssidNumber).c_str(), 14);
          strcat(selectionList, " ip\n");
          crossIndex[selectionCount].code = CI_IP_LIST_INDEX;
          crossIndex[selectionCount].index = ssidNumber;
        } else {
          bool foundMatch = false;
          for (int stationNumber = 1; stationNumber <= WifiStationCount; stationNumber++) {
            wifiManager.setStation(stationNumber);
            if (WiFi.SSID(ssidNumber).equals(wifiManager.sta->ssid)) {
              VF("matched station "); VL(stationNumber);
              if (++selectionCount > crossIndexSize) break;
              if (strlen(wifiManager.sta->host) > 0)
                strncat(selectionList, wifiManager.sta->host, 14);
              else
                strncat(selectionList, wifiManager.sta->ssid, 14);
              strcat(selectionList, " ip\n");
              crossIndex[selectionCount].code = CI_IP_STATION_INDEX;
              crossIndex[selectionCount].index = stationNumber;
              foundMatch = true;
            }
          }
          if (!foundMatch) { VLF("no match"); }
        }

      }
    #endif

    #if SERIAL_BT_MODE != OFF
      BTScanResults *btDeviceList;

      if (!showEditList) {
        VF("MSG: Connect menu, starting asynchronous BT discovery");
        btDeviceList = SERIAL_BT.getScanResults();  // maybe accessing from different threads!
        if (SERIAL_BT.discoverAsync([](BTAdvertisedDevice *pDevice) { VF("."); })) {
          delay(SERIAL_BT_DISCOVERY_TIME);
          VLF("");
          VLF("MSG: Connect menu, stopping asynchronous BT discovery");
          SERIAL_BT.discoverAsyncStop();
          delay(3000);
          scanDone = true;
        }
      }

      if (scanDone) {
        VF("MSG: Connect menu, found "); V(btDeviceList->getCount()); VLF(" BT devices");

        if (btDeviceList->getCount() > 0) {
          for (int deviceNumber = 0; deviceNumber < btDeviceList->getCount(); deviceNumber++) {
            if (strlen(selectionList) + 18 >= sizeof(selectionList)) break;

            BTAdvertisedDevice *device = btDeviceList->getDevice(deviceNumber);

            if (strlen(device->getName().c_str()) != 0) {
              VF("MSG: Connect menu, BT ADDR ");
              VF(device->getAddress().toString().c_str()); VF(" ("); V(device->getName().c_str());

              // VLF(device->getRSSI()); // crashes why?
              std::map<int, std::string> channels = SERIAL_BT.getChannels(device->getAddress());
              VF(" "); V(channels.size()); VF(" svcs -");
              for (auto const &entry: channels) {
                UNUSED(entry);
                VF(" "); V(entry.first); VF(":"); V(entry.second.c_str());
              }

              VF(") ");

              if (showEditList) {
                VLF("");
                if (++selectionCount > crossIndexSize) break;
                strncat(selectionList, device->getName().c_str(), 14);
                strcat(selectionList, " bt\n");
                crossIndex[selectionCount].code = CI_BT_LIST_INDEX;
                crossIndex[selectionCount].index = deviceNumber;
              } else {
                bool foundMatch = false;
                for (int stationNumber = 1; stationNumber <= BluetoothStationCount; stationNumber++) {
                  bluetoothManager.setStation(stationNumber);
                  if (device->getAddress().toString().equals(bluetoothManager.sta->address)) {
                    VF("matched station "); VL(stationNumber);
                    if (++selectionCount > crossIndexSize) break;
                    strncat(selectionList, bluetoothManager.sta->host, 14);
                    strcat(selectionList, " bt\n");
                    crossIndex[selectionCount].code = CI_BT_STATION_INDEX;
                    crossIndex[selectionCount].index = stationNumber;
                    foundMatch = true;
                  }
                }
                if (!foundMatch) { VLF("no match"); }
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

    if (!showEditList) {
      selectionCount++;
      strcat(selectionList, L_SHOW_EDIT "\n");
      showAllSelection = selectionCount;
    } else showAllSelection = -1;

    selectionList[strlen(selectionList) - 1] = 0;

    tasks.yield(500);

    // show the menu
    userSelection = 1;
    userSelection = display->UserInterfaceSelectionList(&keyPad, current_title, userSelection, selectionList);

    VF("MSG: Connect menu, user selected ");

    #if SERIAL_ONSTEP != OFF
      if (!showEditList && userSelection == 1) {
        onStep.connectionMode = CM_SERIAL;
        VLF("Serial");
        return true;
      } else
    #endif

    #if SERIAL_IP_MODE != OFF
      if (crossIndex[userSelection].code == CI_IP_LIST_INDEX) {
        onStep.connectionMode = CM_WIFI;
        VF("WiFi "); VL(crossIndex[userSelection].index);

        VF("MSG: Connect menu, selected device SSID ");
        VL(WiFi.SSID(crossIndex[userSelection].index).c_str());

        menuWiFiStationEditSelect(WiFi.SSID(crossIndex[userSelection].index).c_str());
        goto rescan;
      } else
      if (crossIndex[userSelection].code == CI_IP_STATION_INDEX) {
        VF("WiFi station "); VL(crossIndex[userSelection].index);
        wifiManager.setStation(crossIndex[userSelection].index);
        onStep.connectionMode = CM_WIFI;
        return true;
      } else
    #endif

    #if SERIAL_BT_MODE != OFF
      if (crossIndex[userSelection].code == CI_BT_LIST_INDEX) {
        VF("Bluetooth "); VL(crossIndex[userSelection].index);
        
        BTAdvertisedDevice *device = btDeviceList->getDevice(crossIndex[userSelection].index);
        VF("MSG: Connect menu, selected device ");
        VF(device->getAddress().toString().c_str()); DF(" ");
        VLF(device->getName().c_str());

        menuBTStationEditSelect(device->getName().c_str(), device->getAddress().toString().c_str());
        goto rescan;
      } else
      if (crossIndex[userSelection].code == CI_BT_STATION_INDEX) {

        VF("MSG: Connect menu, scanning "); V(btDeviceList->getCount()); VLF(" BT devices");

        if (btDeviceList->getCount() > 0) {
          VF("Bluetooth station "); VL(crossIndex[userSelection].index);
          bluetoothManager.setStation(crossIndex[userSelection].index);

          for (int deviceNumber = 0; deviceNumber < btDeviceList->getCount(); deviceNumber++) {
            BTAdvertisedDevice *device = btDeviceList->getDevice(deviceNumber);

            if (device->getAddress().toString().equals(bluetoothManager.sta->address)) {

              VF("MSG: Connect menu, selected device ");
              VF(device->getAddress().toString().c_str()); DF(" ");
              VLF(device->getName().c_str());

              int channel = 0;
              BTAddress addr;
              std::map<int, std::string> channels = SERIAL_BT.getChannels(device->getAddress());
              if (channels.size() > 0) { addr = device->getAddress(); channel = channels.begin()->first; }

              if (addr) {
                VF("MSG: Connect menu, connecting to "); V(addr.toString().c_str());
                VF(" channel "); V(channel);

                if (strlen(bluetoothManager.sta->passkey) > 0) {
                  VF(" passkey "); V(bluetoothManager.sta->passkey);
                  SERIAL_BT.setPin(bluetoothManager.sta->passkey);
                }

                VF("...");
                if (SERIAL_BT.connect(addr, channel, ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE)) {
                  VLF(" success");
                  onStep.connectionMode = CM_BLUETOOTH;
                  return true;
                }
              }
            }
          }
        }
      } else
    #endif

    if (userSelection == showAllSelection) {
      VLF("Showing all for editing");
      showEditList = true;
      goto rescan;
    } else

    if (userSelection == 0) {
      if (showEditList) {
        VLF("Showing known");
        showEditList = false;
      }
      goto rescan;
    } else

    {
      V(userSelection);
      VLF(" unknown"); 
    }

    goto rescan;
  }

void UI::menuWiFiStationEditSelect(const char *ssid) {
  char title[32];
  sprintf(title, "WiFi %s", ssid);

  VLF("MSG: Connect menu, selection WiFi station");

  int current_selection = 1;
  while (current_selection != 0) {

    char selection_list[255] = "";
    char selection_item[32];
    
    for (int station = 1; station <= WifiStationCount; station++) {
      sprintf(selection_item, "Memory %d", station);
      strcat(selection_list, selection_item);
      if (station < WifiStationCount) strcat(selection_list, "\n");
    }
    
    current_selection = display->UserInterfaceSelectionList(&keyPad, ssid, current_selection, selection_list);

    if (current_selection != 0) menuWiFiStationEdit(ssid, current_selection);

    Y;
  }
}

void UI::menuWiFiStationEdit(const char *ssid, int index) {
  bool ssidAccepted;
  char title[32];
  char titleIP[32];

  const char *selection_list =
  "Name\n"
  "SSID\n"
  "Password\n"
  "DHCP\n"
  "IP1\n"
  "IP2\n"
  "IP3\n"
  "IP4\n"
  "Mask IP1\n"
  "Mask IP2\n"
  "Mask IP3\n"
  "Mask IP4\n"
  "Target IP1\n"
  "Target IP2\n"
  "Target IP3\n"
  "Target IP4";

  const char *selection_list_dhcp =
  "Name\n"
  "SSID\n"
  "Password\n"
  "DHCP\n"
  "Target IP1\n"
  "Target IP2\n"
  "Target IP3\n"
  "Target IP4";

  VF("MSG: Connect menu, editing WiFi station "); VL(index);

  sprintf(title, "WiFi Memory %d", index);
  wifiManager.setStation(index);

  int current_selection = 1;
  while (current_selection != 0) {

    if (!wifiManager.sta->dhcpEnabled) {
      current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);
      switch (current_selection) {
        case 1:
          display->UserInterfaceInputValueFQDN(&keyPad, "Host Name", "", wifiManager.sta->host, 16, "");
        break;

        case 2:
          message.show("Current SSID:", wifiManager.sta->ssid, 3000);
          ssidAccepted = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, "Overwrite?", &ssidAccepted)) {
            if (ssidAccepted) {
              strcpy(wifiManager.sta->ssid, ssid);
            }
          }
        break;

        case 3:
          display->UserInterfaceInputValuePassword(&keyPad, "Password", "", wifiManager.sta->pwd, ' ', '~', 16, "");
        break;

        case 4:
          display->UserInterfaceInputValueBoolean(&keyPad, "Use DHCP?", &wifiManager.sta->dhcpEnabled);
        break;

        // Station IP
        case 5:
          sprintf(titleIP, "[?].%d.%d.%d", wifiManager.sta->ip[1], wifiManager.sta->ip[2], wifiManager.sta->ip[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->ip[0], 0, 255, 3, "]");
        break;
        case 6:
          sprintf(titleIP, "%d.[?].%d.%d", wifiManager.sta->ip[0], wifiManager.sta->ip[2], wifiManager.sta->ip[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->ip[1], 0, 255, 3, "]");
        break;
        case 7:
          sprintf(titleIP, "%d.%d.[?].%d", wifiManager.sta->ip[0], wifiManager.sta->ip[1], wifiManager.sta->ip[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->ip[2], 0, 255, 3, "]");
        break;
        case 8:
          sprintf(titleIP, "%d.%d.%d.[?]", wifiManager.sta->ip[0], wifiManager.sta->ip[1], wifiManager.sta->ip[2]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->ip[3], 0, 255, 3, "]");
        break;

        // Subnet Mask IP
        case 9:
          sprintf(titleIP, "[?].%d.%d.%d", wifiManager.sta->sn[1], wifiManager.sta->sn[2], wifiManager.sta->sn[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->sn[0], 0, 255, 3, "]");
        break;
        case 10:
          sprintf(titleIP, "%d.[?].%d.%d", wifiManager.sta->sn[0], wifiManager.sta->sn[2], wifiManager.sta->sn[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->sn[1], 0, 255, 3, "]");
        break;
        case 11:
          sprintf(titleIP, "%d.%d.[?].%d", wifiManager.sta->sn[0], wifiManager.sta->sn[1], wifiManager.sta->sn[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->sn[2], 0, 255, 3, "]");
        break;
        case 12:
          sprintf(titleIP, "%d.%d.%d.[?]", wifiManager.sta->sn[0], wifiManager.sta->sn[1], wifiManager.sta->sn[2]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->sn[3], 0, 255, 3, "]");
        break;

        // Target IP
        case 13:
          sprintf(titleIP, "[?].%d.%d.%d", wifiManager.sta->target[1], wifiManager.sta->target[2], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[0], 0, 255, 3, "]");
        break;
        case 14:
          sprintf(titleIP, "%d.[?].%d.%d", wifiManager.sta->target[0], wifiManager.sta->target[2], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[1], 0, 255, 3, "]");
        break;
        case 15:
          sprintf(titleIP, "%d.%d.[?].%d", wifiManager.sta->target[0], wifiManager.sta->target[1], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[2], 0, 255, 3, "]");
        break;
        case 16:
          sprintf(titleIP, "%d.%d.%d.[?]", wifiManager.sta->target[0], wifiManager.sta->target[1], wifiManager.sta->target[2]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[3], 0, 255, 3, "]");
        break;
      }
    } else {
      current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list_dhcp);
      switch (current_selection) {
        case 1:
          display->UserInterfaceInputValueFQDN(&keyPad, "Host Name", "", wifiManager.sta->host, 16, "");
        break;

        case 2:
          message.show("Current SSID:", wifiManager.sta->ssid, 3000);
          ssidAccepted = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, "Overwrite?", &ssidAccepted)) {
            if (ssidAccepted) {
              strcpy(wifiManager.sta->ssid, ssid);
            }
          }
        break;

        case 3:
          display->UserInterfaceInputValuePassword(&keyPad, "Password", "", wifiManager.sta->pwd, ' ', '~', 16, "");
        break;

        case 4:
          display->UserInterfaceInputValueBoolean(&keyPad, "Use DHCP?", &wifiManager.sta->dhcpEnabled);
        break;

        // Target IP
        case 5:
          sprintf(titleIP, "[?].%d.%d.%d", wifiManager.sta->target[1], wifiManager.sta->target[2], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[0], 0, 255, 3, "]");
        break;
        case 6:
          sprintf(titleIP, "%d.[?].%d.%d", wifiManager.sta->target[0], wifiManager.sta->target[2], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[1], 0, 255, 3, "]");
        break;
        case 7:
          sprintf(titleIP, "%d.%d.[?].%d", wifiManager.sta->target[0], wifiManager.sta->target[1], wifiManager.sta->target[3]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[2], 0, 255, 3, "]");
        break;
        case 8:
          sprintf(titleIP, "%d.%d.%d.[?]", wifiManager.sta->target[0], wifiManager.sta->target[1], wifiManager.sta->target[2]);
          display->UserInterfaceInputValueInteger(&keyPad, titleIP, "[", &wifiManager.sta->target[3], 0, 255, 3, "]");
        break;
      }
    }
  }

  // save any changes
  wifiManager.writeSettings();
}

void UI::menuBTStationEditSelect(const char *name, const char *address) {
  char title[32];
  sprintf(title, "BT %s", name);

  VLF("MSG: Connect menu, selecting BT station");

  int current_selection = 1;
  while (current_selection != 0) {

    char selection_list[255] = "";
    char selection_item[32];
    
    for (int station = 1; station <= BluetoothStationCount; station++) {
      sprintf(selection_item, "Memory %d", station);
      strcat(selection_list, selection_item);
      if (station < BluetoothStationCount) strcat(selection_list, "\n");
    }

    current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);

    if (current_selection > 0) menuBTStationEdit(name, address, current_selection);

    Y;
  }
}

void UI::menuBTStationEdit(const char *name, const char *address, int index) {
  bool macAccepted;
  char title[32];

  const char *selection_list =
  "Name/MAC\n"
  "PassKey";

  VF("MSG: Connect menu, editing BT station "); VL(index);

  sprintf(title, "BT Memory %d", index);
  bluetoothManager.setStation(index);

  int current_selection = 1;
  while (current_selection != 0) {
    current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);
    switch (current_selection) {
      case 1:
        message.show("Current Host:", bluetoothManager.sta->host, 3000);
        message.show("Current MAC:", bluetoothManager.sta->address, 3000);
        macAccepted = false;
        if (display->UserInterfaceInputValueBoolean(&keyPad, "Overwrite?", &macAccepted)) {
          if (macAccepted) {
            strcpy(bluetoothManager.sta->host, name);
            strcpy(bluetoothManager.sta->address, address);
          }
        }
      break;
      case 2:
        display->UserInterfaceInputValuePassword(&keyPad, "PassKey", "", bluetoothManager.sta->passkey, '0', '9', 4, "");
      break;
    }
  }

  // save any changes
  bluetoothManager.writeSettings();
}

#endif

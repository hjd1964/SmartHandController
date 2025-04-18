// -----------------------------------------------------------------------------------
// MenuConnect, for UserInterface
#include "../UserInterface.h"
#include "../../lib/tasks/OnTask.h"
#include "../../lib/nv/Nv.h"

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

void UI::menuWireless() {
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
  #if SERIAL_BT_MODE != OFF
    BTScanResults *btDeviceList = NULL;
    UNUSED(btDeviceList);
    SERIAL_BT.begin(SERIAL_BT_NAME, true);
  #endif

  // reset the reconnection count
  reconnectionCount = -1;

rescan:
  // build the menu selections
  selectionCount = 0;
  selectionList[0] = 0;
  for (int i = 0; i < crossIndexSize; i++) crossIndex[i].code = CI_NONE;

  if (showEditList) {
    strcpy(current_title, L_WIFI_SELECT_EDIT);
    message.show(L_UPDATING, L_PLEASE_WAIT "...", 250);
  } else {
    strcpy(current_title, L_WIFI_SELECT);
    message.show(L_SCANNING, L_PLEASE_WAIT "...", 10);
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
            if (!foundMatch) { VF("matched station(s) "); foundMatch = true; }
            V(stationNumber); V(" ");
            if (++selectionCount > crossIndexSize) break;
            if (strlen(wifiManager.sta->host) > 0)
              strncat(selectionList, wifiManager.sta->host, 14);
            else
              strncat(selectionList, wifiManager.sta->ssid, 14);
            strcat(selectionList, " ip\n");
            crossIndex[selectionCount].code = CI_IP_STATION_INDEX;
            crossIndex[selectionCount].index = stationNumber;
          }
        }
        if (!foundMatch) { VF("no match"); }
        VLF("");
      }

    }
  #endif

  #if SERIAL_BT_MODE != OFF

    if (!showEditList) {
      VF("MSG: Connect menu, starting asynchronous BT discovery");
      btDeviceList = SERIAL_BT.getScanResults();
      if (SERIAL_BT.discoverAsync([](BTAdvertisedDevice *pDevice) { VF("."); })) {
        delay(SERIAL_BT_DISCOVERY_TIME);
        VLF("");
        VLF("MSG: Connect menu, stopping asynchronous BT discovery");
        SERIAL_BT.discoverAsyncStop();
        delay(3000);
      }
    }

    if (btDeviceList != NULL) {
      VF("MSG: Connect menu, found "); V(btDeviceList->getCount()); VLF(" BT devices");

      if (btDeviceList->getCount() > 0) {
        for (int deviceNumber = 0; deviceNumber < btDeviceList->getCount(); deviceNumber++) {
          if (strlen(selectionList) + 18 >= sizeof(selectionList)) break;

          BTAdvertisedDevice *device = btDeviceList->getDevice(deviceNumber);

          if (strequ(HOST_NAME, device->getName().c_str())) {
            VF("MSG: Connect menu, skipping SHC BT Host "); VL(device->getName().c_str());
            continue;
          }

          if (strlen(device->getName().c_str()) != 0) {
            VF("MSG: Connect menu, BT ADDR ");
            VF(device->getAddress().toString().c_str()); VF(" ("); V(device->getName().c_str());
            // VLF(device->getRSSI()); // crashes why?

            if (DEBUG != OFF) {
              std::map<int, std::string> channels = SERIAL_BT.getChannels(device->getAddress());
              VF(" "); V(channels.size()); VF(" svcs -");
              for (auto const &entry: channels) {
                UNUSED(entry);
                VF(" "); V(entry.first); VF(":"); V(entry.second.c_str());
              }
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
      VLF("Serial");

      #if REBOOT_TO_SERIAL == ON
        onStep.connectionMode = CM_SERIAL;
        VLF("MSG: Connect menu, setting boot flag for Serial mode (restarting...)");
        message.show(L_CONNECTING, L_PLEASE_WAIT "...", 10);
        nv.write(NV_SERIAL_BOOT_FLAG_BASE, (uint8_t)CS_SERIAL);
        tasks.yield(NV_WAIT + 500);
        HAL_RESET();
      #else
        onStep.connectionMode = CM_SERIAL;
        connectionSelection = CS_SERIAL;
        return;
      #endif
    } else
  #endif

  #if SERIAL_IP_MODE != OFF
    if (crossIndex[userSelection].code == CI_IP_LIST_INDEX) {
      VF("WiFi "); VL(crossIndex[userSelection].index);

      VF("MSG: Connect menu, selected device SSID ");
      VL(WiFi.SSID(crossIndex[userSelection].index).c_str());

      menuWiFiStationEditSelect(WiFi.SSID(crossIndex[userSelection].index).c_str());
      goto rescan;
    } else
    if (crossIndex[userSelection].code == CI_IP_STATION_INDEX) {
      VF("WiFi station "); VL(crossIndex[userSelection].index);

      #if REBOOT_TO_WIFI == ON
        VF("MSG: Connect menu, setting boot flag for WiFi station "); V(crossIndex[userSelection].index); VLF(" (restarting...)");
        message.show(L_CONNECTING, L_PLEASE_WAIT "...", 10);
        nv.write(NV_SERIAL_BOOT_FLAG_BASE, (uint8_t)CS_WIFI_STA1 + (uint8_t)(crossIndex[userSelection].index - 1));
        tasks.yield(NV_WAIT + 500);
        HAL_RESET();
      #else
        onStep.connectionMode = CM_WIFI;
        connectionSelection = (ConnectSelection)((uint8_t)CS_WIFI_STA1 + (uint8_t)(crossIndex[userSelection].index - 1));
        return;
      #endif
    } else
  #endif

  #if SERIAL_BT_MODE != OFF
    if (crossIndex[userSelection].code == CI_BT_LIST_INDEX && btDeviceList != NULL) {
      VF("Bluetooth "); VL(crossIndex[userSelection].index);
      
      BTAdvertisedDevice *device = btDeviceList->getDevice(crossIndex[userSelection].index);
      VF("MSG: Connect menu, selected device ");
      VF(device->getAddress().toString().c_str()); DF(" ");
      VLF(device->getName().c_str());

      menuBTStationEditSelect(device->getName().c_str(), device->getAddress().toString().c_str());
      goto rescan;
    } else
    if (crossIndex[userSelection].code == CI_BT_STATION_INDEX) {
      VF("Bluetooth station "); VL(crossIndex[userSelection].index);

      VF("MSG: Connect menu, scanning "); V(btDeviceList->getCount()); VLF(" BT devices");

      if (btDeviceList->getCount() > 0) {
        VF("Bluetooth station "); VL(crossIndex[userSelection].index);
        bluetoothManager.setStation(crossIndex[userSelection].index);

        for (int deviceNumber = 0; deviceNumber < btDeviceList->getCount(); deviceNumber++) {
          BTAdvertisedDevice *device = btDeviceList->getDevice(deviceNumber);

          if (device->getAddress().toString().equals(bluetoothManager.sta->address)) {
            #if REBOOT_TO_BLUETOOTH == ON
              VF("MSG: Connect menu, setting boot flag for BT station "); V(crossIndex[userSelection].index); VLF(" (restarting...)");
              message.show(L_CONNECTING, L_PLEASE_WAIT "...", 10);
              nv.write(NV_SERIAL_BOOT_FLAG_BASE, (uint8_t)CS_BT_STA1 + (uint8_t)(crossIndex[userSelection].index - 1));
              tasks.yield(NV_WAIT + 500);
              HAL_RESET();
            #else
              onStep.connectionMode = CM_BLUETOOTH;
              connectionSelection = (ConnectSelection)((uint8_t)CS_BT_STA1 + (uint8_t)(crossIndex[userSelection].index - 1));
              return;
            #endif
          }
        }
      }
    } else
  #endif

  if (userSelection == showAllSelection) {
    VLF("showing all for editing");
    showEditList = true;
    goto rescan;
  } else

  if (userSelection == 0) {
    if (showEditList) {
      VLF("show known");
      showEditList = false;
    } else {
      VLF("refresh");
    }
    goto rescan;
  } else

  {
    V(userSelection);
    VLF(" unknown"); 
  }

  goto rescan;
}

#if SERIAL_IP_MODE != OFF
  void UI::menuWiFiStationEditSelect(const char *ssid) {
    char title[32];
    sprintf(title, "WiFi %s", ssid);

    VLF("MSG: Connect menu, selection WiFi station");

    int current_selection = 1;
    while (current_selection != 0) {

      char selection_list[255] = "";
      char selection_item[32];
      
      for (int station = 1; station <= WifiStationCount; station++) {
        wifiManager.setStation(station);
        char inUseIndication = ' ';
        if (strlen(wifiManager.sta->ssid) > 0) inUseIndication = '~';
        if (strequ(wifiManager.sta->ssid, ssid)) inUseIndication = '<';
        sprintf(selection_item, L_MEMORY " %d%c", station, inUseIndication);
        strcat(selection_list, selection_item);
        if (station < WifiStationCount) strcat(selection_list, "\n");
      }
      
      current_selection = display->UserInterfaceSelectionList(&keyPad, ssid, current_selection, selection_list);

      if (current_selection != 0) menuWiFiStationEdit(ssid, current_selection);

      Y;
    }
  }

  void UI::menuWiFiStationEdit(const char *ssid, int index) {
    bool accept;
    char title[32];

    const char *selection_list =
    "Hostname\n"    // 1
    "SSID\n"        // 2
    L_PASSWORD "\n" // 3
    "DHCP\n"        // 4
    "SHC IP\n"      // 5
    "Gateway IP\n"  // 6
    "Mask IP\n"     // 7
    "Target IP\n"   // 8
    "Clear";        // 9

    const char *selection_list_dhcp =
    "Hostname\n"     // 1
    "SSID\n"         // 2
    L_PASSWORD "\n"  // 3
    "DHCP\n"         // 4
    "Target IP\n"    // 5 + 3
    "Clear";         // 6 + 3

    VF("MSG: Connect menu, editing WiFi station "); VL(index);

    sprintf(title, "WiFi " L_MEMORY " %d", index);
    wifiManager.setStation(index);

    int current_selection = 1;
    while (current_selection != 0) {
      if (wifiManager.sta->dhcpEnabled) {
        current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list_dhcp);
      } else {
        current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);
      }

      if (wifiManager.sta->dhcpEnabled && current_selection >= 5) current_selection += 3;

      switch (current_selection) {
        case 1:
          accept = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, "SSID as Hostname?", &accept)) {
            if (accept) sstrcpy(wifiManager.sta->host, ssid, 32);
            display->UserInterfaceInputValueFQDN(&keyPad, "Hostname", "", wifiManager.sta->host, 32, "");
          }
        break;

        case 2:
          message.show(L_CURRENT " SSID:", wifiManager.sta->ssid, 3000);
          accept = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, L_OVERWRITE "?", &accept)) {
            if (accept) {
              sstrcpy(wifiManager.sta->ssid, ssid, 33);
            }
          }
        break;

        case 3:
          display->UserInterfaceInputValuePassword(&keyPad, L_PASSWORD, "", wifiManager.sta->pwd, ' ', '~', 63, "");
        break;

        case 4:
          message.show(L_DHCP_LINE1, L_DHCP_LINE2, 3000);
          display->UserInterfaceInputValueBoolean(&keyPad, L_USE " DHCP?", &wifiManager.sta->dhcpEnabled);
        break;

        case 5:
          display->UserInterfaceInputValueIP(&keyPad, "SHC IP", wifiManager.sta->ip);
        break;

        case 6:
          display->UserInterfaceInputValueIP(&keyPad, "Gateway IP", wifiManager.sta->gw);
        break;

        case 7:
          display->UserInterfaceInputValueIP(&keyPad, "Subnet IP", wifiManager.sta->sn);
        break;

        case 8:
          display->UserInterfaceInputValueIP(&keyPad, "Target IP", wifiManager.sta->target);
        break;

        // Reset all
        case 9:
          char title[32];
          sprintf(title, L_CLEAR " " L_MEMORY " %d ?", index);
          accept = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, title, &accept)) {
            if (accept) {
              accept = false;
              if (display->UserInterfaceInputValueBoolean(&keyPad, L_CONFIRM, &accept)) {
                if (accept) {
                  strcpy(wifiManager.sta->host, "");
                  strcpy(wifiManager.sta->ssid, "");
                  strcpy(wifiManager.sta->pwd, "");
                  wifiManager.sta->dhcpEnabled = false;
                  ip4toip4(wifiManager.sta->gw, IPAddress(192,168,0,1));
                  ip4toip4(wifiManager.sta->ip, IPAddress(192,168,0,1));
                  ip4toip4(wifiManager.sta->sn, IPAddress(255,255,255,0));
                  ip4toip4(wifiManager.sta->target, IPAddress(192,168,0,1));
                }
              }
            }
          }
        break;
      }

      if (wifiManager.sta->dhcpEnabled && current_selection >= 8) current_selection -= 3;

    }

    // save any changes
    wifiManager.writeSettings();
  }
#endif

#if SERIAL_BT_MODE != OFF
  void UI::menuBTStationEditSelect(const char *name, const char *address) {
    char title[32];
    sprintf(title, "BT %s", name);

    VLF("MSG: Connect menu, selecting BT station");

    int current_selection = 1;
    while (current_selection != 0) {

      char selection_list[255] = "";
      char selection_item[32];
      
      for (int station = 1; station <= BluetoothStationCount; station++) {
        bluetoothManager.setStation(station);
        char inUseIndication = ' ';
        if (strlen(bluetoothManager.sta->address) > 0) inUseIndication = '~';
        if (strequ(bluetoothManager.sta->address, address)) inUseIndication = '<';
        sprintf(selection_item, L_MEMORY " %d%c", station, inUseIndication);
        strcat(selection_list, selection_item);
        if (station < BluetoothStationCount) strcat(selection_list, "\n");
      }

      current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);

      if (current_selection > 0) menuBTStationEdit(name, address, current_selection);

      Y;
    }
  }

  void UI::menuBTStationEdit(const char *name, const char *address, int index) {
    bool accept;
    char title[32];

    const char *selection_list =
    "Name/MAC\n"
    "PassKey\n"
    L_CLEAR;

    VF("MSG: Connect menu, editing BT station "); VL(index);

    sprintf(title, "BT " L_MEMORY " %d", index);
    bluetoothManager.setStation(index);

    int current_selection = 1;
    while (current_selection != 0) {
      current_selection = display->UserInterfaceSelectionList(&keyPad, title, current_selection, selection_list);
      switch (current_selection) {
        // Set the MAC and Hostname
        case 1:
          message.show(L_CURRENT " Host:", bluetoothManager.sta->host, 3000);
          message.show(L_CURRENT " MAC:", bluetoothManager.sta->address, 3000);
          accept = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, L_OVERWRITE "?", &accept)) {
            if (accept) {
              strcpy(bluetoothManager.sta->host, name);
              strcpy(bluetoothManager.sta->address, address);
            }
          }
        break;

        // PassKey aka Pin
        case 2:
          display->UserInterfaceInputValuePassword(&keyPad, "PassKey", "", bluetoothManager.sta->passkey, '0', '9', 4, "");
        break;

        // Reset all
        case 3:
          char title[32];
          sprintf(title, L_CLEAR " " L_MEMORY " %d ?", index);
          accept = false;
          if (display->UserInterfaceInputValueBoolean(&keyPad, title, &accept)) {
            if (accept) {
              accept = false;
              if (display->UserInterfaceInputValueBoolean(&keyPad, L_CONFIRM, &accept)) {
                if (accept) {
                  strcpy(bluetoothManager.sta->host, "");
                  strcpy(bluetoothManager.sta->address, "");
                  strcpy(bluetoothManager.sta->passkey, "");
                }
              }
            }
          }
        break;
      }
    }

    // save any changes
    bluetoothManager.writeSettings();
  }
#endif

#endif

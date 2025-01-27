#pragma once

// host name for this microcontroller, by default used for the following if enabled/supported:
// PRODUCT_DESCRIPTION    the user friendly name for this device, appears on websites etc.
// HOST_NAME              the name ESP WiFi provides to any DHCP server (Ethernet doesn't support this)
#ifndef HOST_NAME
#define HOST_NAME                "OnStep-SHC"
#endif

// use the HAL specified default NV driver
#ifndef NV_DRIVER
#define NV_DRIVER                NV_DEFAULT
#define NV_WAIT                  1500
#endif

// activate ST4 port serial interface
#define SERIAL_ST4
#define SERIAL_ST4_SLAVE         ON
#define SERIAL_ST4_SLAVE_PRESENT

#define SERIAL_BAUD              SERIAL_ONSTEP_BAUD_DEFAULT
#define SERIAL_BAUD_DEFAULT      SERIAL_ONSTEP_BAUD_DEFAULT

#ifndef SERIAL_RADIO
#define SERIAL_RADIO             OFF
#endif

// enable and customize WiFi functionality
// for other default IP settings see the file:
// src/lib/wifi/WifiManager.defaults.h
#if SERIAL_RADIO == WIFI_STATION || SERIAL_RADIO == BOTH
#define SERIAL_IP_MODE           STATION
#ifndef WifiStationCount
// Use all six stations
#define WifiStationCount         6
#endif
#endif

// some additional flexibility, just incase

// note BT reboots to the connect menu regardless of the setting below
// which reboots serial and wifi also
#ifndef REBOOT_TO_CONNECT_MENU
#define REBOOT_TO_CONNECT_MENU   OFF
#endif
#ifndef REBOOT_TO_SERIAL
#define REBOOT_TO_SERIAL         OFF
#endif
#ifndef REBOOT_TO_WIFI
#define REBOOT_TO_WIFI           OFF
#endif
#ifndef REBOOT_TO_BLUETOOTH
#define REBOOT_TO_BLUETOOTH      OFF
#endif

#ifndef SERIAL_IP_MODE
#define SERIAL_IP_MODE           OFF
#endif

#if SERIAL_IP_MODE == STATION
// first, you must have an Wifi or Ethernet device:  OFF or WIFI, ETHERNET_W5100, ETHERNET_W5500
#define OPERATIONAL_MODE         WIFI

// optional Arduino Serial class work-alike IP channel to port 9996 as a client (connects to a server)
#define SERIAL_CLIENT            ON

// enable station mode and set default to connect to an OnStep
#define STA_ENABLED              true
#define STA_HOST_NAME            "OnStep"
#define STA_SSID                 "OnStep"

#ifndef MDNS_SERVER
#define MDNS_SERVER              ON
#endif
#ifndef MDNS_CLIENT
#define MDNS_CLIENT              ON
#endif

#ifndef STA1_DHCP_ENABLED
#define STA1_DHCP_ENABLED        true
#endif
#ifndef STA2_DHCP_ENABLED
#define STA2_DHCP_ENABLED        true
#endif
#ifndef STA3_DHCP_ENABLED
#define STA3_DHCP_ENABLED        true
#endif
#ifndef STA4_DHCP_ENABLED
#define STA4_DHCP_ENABLED        true
#endif
#ifndef STA5_DHCP_ENABLED
#define STA5_DHCP_ENABLED        true
#endif
#ifndef STA6_DHCP_ENABLED
#define STA6_DHCP_ENABLED        true
#endif
#endif

// enable and customize Bluetooth functionality
// for other default BT settings see the file:
// src/lib/bluetooth/BluetoothManager.defaults.h
#if SERIAL_RADIO == BLUETOOTH || SERIAL_RADIO == BOTH
#define SERIAL_BT_MODE           MASTER
#define SERIAL_BT_NAME           HOST_NAME
#endif

#ifndef SERIAL_BT_MODE
#define SERIAL_BT_MODE           OFF
#endif

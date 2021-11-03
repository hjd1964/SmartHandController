#pragma once

// activate ST4 port serial interface
#define SERIAL_ST4
#define SERIAL_ST4_SLAVE ON
#define SERIAL_ST4_SLAVE_PRESENT

// setup for Wifi IP communications to OnStep
// for other settings see the WiFiManager class

#ifndef SERIAL_IP_MODE
  #define SERIAL_IP_MODE OFF
#endif

#if SERIAL_IP_MODE == STATION
  // first, you must have an Wifi or Ethernet device:  OFF or WIFI, ETHERNET_W5100, ETHERNET_W5500
  #define OPERATIONAL_MODE       WIFI

  // optional Arduino Serial class work-alike IP channel to port 9998 as a client (connects to a server)
  #define SERIAL_CLIENT          ON

  // set station mode
  #define STA_ENABLED            true

  // optional alternate fallback
  #if ONSTEP_ALT_FALLBACK == ON
  #define STA_ALT_FALLBACK       true
  #endif

  // use friendly names for OnStep IP address, SSID, password
  #define STA_DHCP_ENABLED       true
  #define STA_SSID               ONSTEP_SSID
  #define STA_PASSWORD           ONSTEP_PASSWORD
  #define STA_TARGET_IP_ADDR     ONSTEP_IP_ADDR

  #define STA_DHCP_ENABLED_ALT   true
  #define STA_SSID_ALT           ONSTEP_SSID_ALT
  #define STA_PASSWORD_ALT       ONSTEP_PASSWORD_ALT
  #define STA_TARGET_IP_ADDR_ALT ONSTEP_IP_ADDR_ALT
#endif

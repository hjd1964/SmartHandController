#pragma once

// use the HAL specified default NV driver
#ifndef NV_DRIVER
#define NV_DRIVER                     NV_DEFAULT
#endif

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

  // optional Arduino Serial class work-alike IP channel to port 9996 as a client (connects to a server)
  #define SERIAL_CLIENT          ON

  // set station mode
  #define STA_ENABLED            true

  // use friendly names for OnStep IP address, SSID, password
  #define STA1_DHCP_ENABLED      true
  #define STA1_HOST_NAME         ONSTEP1_HOST_NAME
  #define STA1_SSID              ONSTEP1_SSID
  #define STA1_PASSWORD          ONSTEP1_PASSWORD
  #define STA1_TARGET_IP_ADDR    ONSTEP1_IP_ADDR

  #define STA2_DHCP_ENABLED      true
  #define STA2_HOST_NAME         ONSTEP2_HOST_NAME
  #define STA2_SSID              ONSTEP2_SSID
  #define STA2_PASSWORD          ONSTEP2_PASSWORD
  #define STA2_TARGET_IP_ADDR    ONSTEP2_IP_ADDR

  #define STA3_DHCP_ENABLED      true
  #define STA3_HOST_NAME         ONSTEP3_HOST_NAME
  #define STA3_SSID              ONSTEP3_SSID
  #define STA3_PASSWORD          ONSTEP3_PASSWORD
  #define STA3_TARGET_IP_ADDR    ONSTEP3_IP_ADDR
#endif

#pragma once

// host name for this microcontroller
#ifndef HOST_NAME
#define HOST_NAME                "OnStep-SHC"
#endif

// settings identification
#ifndef CONFIG_NAME
#define CONFIG_NAME              HOST_NAME
#endif

// use the HAL specified default NV driver
#ifndef NV_DRIVER
#define NV_DRIVER                NV_DEFAULT
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

// setup for Wifi IP communications to OnStep
// for other settings see the WiFiManager class
#if SERIAL_RADIO == WIFI_STATION
#define SERIAL_IP_MODE           STATION
#endif

// setup for Bluetooth communications to OnStep
#if SERIAL_RADIO == BLUETOOTH
#define SERIAL_BT_MODE           MASTER
#define SERIAL_BT_NAME           HOST_NAME
#endif

// setup for either WiFi or Bluetooth communications to OnStep
#if SERIAL_RADIO == BOTH
#define SERIAL_IP_MODE           STATION
#define SERIAL_BT_MODE           MASTER
#define SERIAL_BT_NAME           HOST_NAME
#endif

// set defaults just incase
#ifndef SERIAL_IP_MODE
#define SERIAL_IP_MODE           OFF
#endif

#ifndef SERIAL_BT_MODE
#define SERIAL_BT_MODE           OFF
#endif

#if SERIAL_IP_MODE == STATION
// first, you must have an Wifi or Ethernet device:  OFF or WIFI, ETHERNET_W5100, ETHERNET_W5500
#define OPERATIONAL_MODE         WIFI

// optional Arduino Serial class work-alike IP channel to port 9996 as a client (connects to a server)
#define SERIAL_CLIENT            ON

// set station mode
#define STA_ENABLED              true
#endif

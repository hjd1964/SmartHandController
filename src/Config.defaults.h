
// setup for Wifi IP communications to OnStep
// for other settings see the WiFiManager class

// use friendly name for OnStep IP address
#ifdef ONSTEP_IP_ADDR
#define TARGET_IP_ADDR1 ONSTEP_IP_ADDR
#endif

#ifdef ONSTEP_SSID
#define STA_SSID ONSTEP_SSID
#endif

#ifdef ONSTEP_PASSWORD
#define STA_PASSWORD ONSTEP_PASSWORD
#endif

// enable wifi ip operation if serial IP mode is selected
#if SERIAL_IP_MODE != OFF && defined(ESP32)
#define OPERATIONAL_MODE WIFI
#endif

// and it's a serial ip client (not server)
#define SERIAL_IP_CLIENT

// use the standard ip connection on port 9999
#ifndef STANDARD_IPSERIAL_CHANNEL
#define STANDARD_IPSERIAL_CHANNEL ON
#endif

#ifndef STA_ENABLED
#define STA_ENABLED true
#endif

// use LAN DHCP addresses
#ifndef STA_DHCP_ENABLED
#define STA_DHCP_ENABLED true
#endif

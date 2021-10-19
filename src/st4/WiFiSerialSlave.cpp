// -----------------------------------------------------------------------------------
// WiFi ST4 slave

#include "WiFiSerialSlave.h"

#if defined(ESP32) && (SERIAL_INTERFACE == SerialWiFi) // only add SerialWiFi code on ESP32 and when it is really used
void SWiFi::connect(char* ssid, char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssid, password);
    DL("Starting WiFi connection to Onstep(X)."); 
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        D(".");
    }
    D("WiFi established. IP of SHC is "); DL(WiFi.localIP());

    // Connect to persistent command server
    IPAddress onstep(192,168,0,1);
    if (!cmdSvrClient.connect(onstep, 9998)) {
        DL("Connection to OnStep(X) persistent CmdServer failed."); 
    }
}

void SWiFi::begin(long baudRate) { 
    connect( ONSTEP_SSID, ONSTEP_PASSWORD); // code works, but need to fix ISO/C++ warning about invalid string to char conversion. 
}

void SWiFi::end() {
    cmdSvrClient.stop();
    DL("CmdServer connection closed.");
    WiFi.disconnect();
    DL("Wifi disconnected.");
}

void SWiFi::paused(bool state){
    // dummy, not needed for WiFi implementation
}

bool SWiFi::active(){
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    } else {
        return false;
    }
}

size_t SWiFi::write(uint8_t data) {
    if (cmdSvrClient.connected()) {
        D("LX200 command: "); DL((const char)data);
        cmdSvrClient.println((const char)data);
    } 

  return 1; 
}

size_t SWiFi::write(const uint8_t *data, size_t quantity) {
    if (cmdSvrClient.connected()) {
        D("LX200 command: "); DL((const char*)data);
        cmdSvrClient.println((const char*)data);
    } 
  return 1;
}

int SWiFi::available(void) {
    int a;
    a = cmdSvrClient.available();
    return a;
}

int SWiFi::read(void) {
  int c = cmdSvrClient.read();
  //D("OnStep response: "); DL(c);
  if (c == 0) c = -1;
  return c;
}

int SWiFi::peek(void) {
    // dummy, not needed for WiFi implementation
    return 1;
}

void SWiFi::flush(void) {
    // dummy, not needed for WiFi implementation
}

SWiFi SerialWiFi;
#endif
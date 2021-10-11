// -----------------------------------------------------------------------------------
// WiFi ST4 slave
//
// Uses persistent command channel
//
#pragma once

#if defined(ESP32)
#include <Stream.h>
#include "../Common.h"
#include <WiFi.h>

class SWiFi : public Stream {
  public:
    void begin(long baudRate);
    void end();
    void paused(bool state);
    bool active();

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;


  private:
    void connect(char* ssid, char* password);
    WiFiClient cmdSvrClient;
};

extern SWiFi SerialWiFi;
#endif
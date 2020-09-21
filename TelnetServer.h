#pragma once
#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#else
  #error Platform not supported
#endif

#define MAX_TELNET_CLIENTS 2

class TelnetServer : public WiFiServer{
  private:
    WiFiClient TelnetClient[MAX_TELNET_CLIENTS];
    bool ConnectionEstablished; // Flag for successfully handled connection

  public:
    TelnetServer(int port = 23);
    void handle();
    size_t write(uint8_t b);
    size_t write(const uint8_t *buffer, size_t size);
};

#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>

#define MAX_TELNET_CLIENTS 2

class TelnetServer : public WiFiServer{
  private:
    WiFiClient TelnetClient[MAX_TELNET_CLIENTS];
    bool ConnectionEstablished; // Flag for successfully handled connection

  public:
    TelnetServer(int port);
    void TelnetMsg(String text);
    void handle();
};

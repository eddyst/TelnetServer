#include "TelnetServer.h"

TelnetServer::TelnetServer(int port):WiFiServer( port ){
}

size_t TelnetServer::write(uint8_t b) {
    return write(&b, 1);
}

size_t TelnetServer::write(const uint8_t *buffer, size_t size) {
  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] || TelnetClient[i].connected())
    {
      TelnetClient[i].write(buffer, size);
      TelnetClient[i].flush();
    }
  }
  return size;
}

void TelnetServer::handle(){
  // Cleanup disconnected session
  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] && !TelnetClient[i].connected())
    {
      #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 3
        DEBUG_SERIAL.print("Client disconnected ... terminate session "); DEBUG_SERIAL.println(i+1); 
      #endif
      TelnetClient[i].stop();
    }
  }
  
  // Check new client connections
  if (hasClient())
  {
    ConnectionEstablished = false; // Set to false
    
    for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
    {
      #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 5
        DEBUG_SERIAL.print("Checking telnet session "); DEBUG_SERIAL.println(i+1);
      #endif

      // find free socket
      if (!TelnetClient[i])
      {
        TelnetClient[i] = available(); 
        
        #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 3
          DEBUG_SERIAL.print("New Telnet client connected to session "); DEBUG_SERIAL.println(i+1);
        #endif

        TelnetClient[i].flush();  // clear input buffer, else you get strange characters
        TelnetClient[i].println("Welcome!");
        
        TelnetClient[i].print("Millis since start: ");
        TelnetClient[i].println(millis());
        
        TelnetClient[i].print("Free Heap RAM: ");
        TelnetClient[i].println(ESP.getFreeHeap());
  
        TelnetClient[i].println("----------------------------------------------------------------");
        
        ConnectionEstablished = true; 
        
        break;
      }
      else
      {
        #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 5
          DEBUG_SERIAL.println("Session is in use");
        #endif
      }
    }

    if (ConnectionEstablished == false)
    {
      #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 3
        DEBUG_SERIAL.println("No free sessions ... drop connection");
      #endif
      available().stop();
      // TelnetMsg("An other user cannot connect ... MAX_TELNET_CLIENTS limit is reached!");
    }
  }

  for(uint8_t i = 0; i < MAX_TELNET_CLIENTS; i++)
  {
    if (TelnetClient[i] && TelnetClient[i].connected())
    {
      if(TelnetClient[i].available())
      { 
        //get data from the telnet client
        while(TelnetClient[i].available())
        {
          #if defined DEBUG_SERIAL && DEBUG_TELNET_Verbose >= 3
            DEBUG_SERIAL.write(TelnetClient[i].read());
          #else
            TelnetClient[i].read();
          #endif
        }
      }
    }
  }
}

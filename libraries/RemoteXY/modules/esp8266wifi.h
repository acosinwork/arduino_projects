#ifndef _REMOTEXY_MOD_ESP8266WIFI_H_
#define _REMOTEXY_MOD_ESP8266WIFI_H_

/*
for ESP8266 board;
need include <ESP8266WiFi.h>;
*/

#include "classes/RemoteXY_API.h"

#define REMOTEXY_SEND_BUFFER_LENGTH 256

class CRemoteXY : public CRemoteXY_API {

  protected:
  uint16_t port;
  char * wifiSsid;
  char * wifiPassword;
  
  WiFiServer *server;
  WiFiClient client;
  
  uint8_t sendBuffer[REMOTEXY_SEND_BUFFER_LENGTH];
  uint16_t sendBufferCount; 
  uint16_t sendBytesAvailable;  

  public:
  CRemoteXY (const void * _conf, void * _var, char * _wifiSsid, char * _wifiPassword, uint16_t _port) {
    port = _port;
    wifiSsid = _wifiSsid;
    wifiPassword = _wifiPassword;
    init (_conf, _var);    
  }
  
  uint8_t initModule () {  
    delay(100);
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
#if defined(REMOTEXY_WIFI__POINT)
    /* access point */
    WiFi.mode(WIFI_AP);
    WiFi.softAP(wifiSsid, wifiPassword);
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.println();
    REMOTEXY__DEBUGLOGS.print("IP: ");
    REMOTEXY__DEBUGLOGS.println(WiFi.softAPIP());
#endif
#else    
    /* station */
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSsid, wifiPassword);
    uint8_t i = 40;
    while (WiFi.status() != WL_CONNECTED && i--) delay(500);
    
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.println();
    if (i) {
      REMOTEXY__DEBUGLOGS.print("IP: ");
      REMOTEXY__DEBUGLOGS.println(WiFi.localIP());
    }
    else {
      REMOTEXY__DEBUGLOGS.print("Could not connect to ");
      REMOTEXY__DEBUGLOGS.println(wifiSsid);
    }
#endif
   if (!i) return 0;

#endif
    client.stop();
    server = new WiFiServer (port);
    server->begin();    
    server->setNoDelay(true);
    return 1;
  }

  void sendStart (uint16_t len) {
    sendBytesAvailable = len;
    sendBufferCount = 0;
  }

  void sendByte (uint8_t b) {
    if (client) {
      if (client.connected()) {
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print (b, HEX);
        REMOTEXY__DEBUGLOGS.print (' ');
#endif
        sendBuffer[sendBufferCount++] = b;
        sendBytesAvailable--;       
        if ((sendBufferCount==REMOTEXY_SEND_BUFFER_LENGTH) || (sendBytesAvailable==0)) {
          uint8_t buf[sendBufferCount];
          for (uint16_t i=0; i<sendBufferCount; i++) buf[i]=sendBuffer[i];
          client.write(buf, sendBufferCount);
          sendBufferCount=0;
        } 
      }
    }
  }  
  
  uint8_t receiveByte () {
    uint8_t b;
    if (client) {
      if (client.connected()) {
        b = client.read();
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print (b, HEX);
        REMOTEXY__DEBUGLOGS.print (' ');
#endif
        return b;
      }
    }
  }
  
  uint8_t availableByte () {
    if (!client) {
      client=server->available();
    }
    if (client) {
      if (client.connected()) return client.available();
      else client.stop();
    }
    return 0;
  }  

};


#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD, REMOTEXY_SERVER_PORT)



#endif //_REMOTEXY_MOD_ESP8266WIFI_H_
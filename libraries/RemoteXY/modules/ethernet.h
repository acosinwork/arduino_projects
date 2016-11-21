#ifndef _REMOTEXY_MOD_ETHERNET_H_
#define _REMOTEXY_MOD_ETHERNET_H_

#include "classes/RemoteXY_API.h"

class CRemoteXY : public CRemoteXY_API {

  protected:
  EthernetServer *server;
  EthernetClient client;
  
  public:
  CRemoteXY (const void * _conf, void * _var, char * _mac, uint16_t _port) {
    uint8_t mac[6];
    init (_conf, _var);
    rxy_getMacAddr (_mac, mac);  
    Ethernet.begin(mac);
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.print("IP: ");
    REMOTEXY__DEBUGLOGS.println(Ethernet.localIP());
#endif
    client.stop();
    server = new EthernetServer (_port);
    server->begin();    
  }
  
  
  void sendByte (uint8_t b) {
    if (client) {
      if (client.connected()) {
        client.write(b);  
#if defined(REMOTEXY__DEBUGLOGS)
        REMOTEXY__DEBUGLOGS.print (b, HEX);
        REMOTEXY__DEBUGLOGS.print (' ');
#endif
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

#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_ETHERNET_MAC, REMOTEXY_SERVER_PORT)


#endif //_REMOTEXY_MOD_ETHERNET_H_
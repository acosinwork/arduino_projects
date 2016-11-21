#ifndef _REMOTEXY_MOD_SERIAL_H_
#define _REMOTEXY_MOD_SERIAL_H_

#include "classes/RemoteXY_Serial.h"
     

class CRemoteXY : public CRemoteXY_Serial {

public:

#if defined(REMOTEXY_PORT__HARDSERIAL)
  CRemoteXY (const void * _conf, void * _var, HardwareSerial * _serial, long _serialSpeed) {
    initSerial (_serial, _serialSpeed);
    init (_conf, _var); 
  }
#elif defined(REMOTEXY_PORT__SOFTSERIAL)
  CRemoteXY (const void * _conf, void * _var, uint8_t _serialRx, uint8_t _serialTx, long _serialSpeed) {
    initSerial (_serialRx, _serialTx, _serialSpeed);
    init (_conf, _var); 
  }
#endif

  protected:

  void sendByte (uint8_t b) {
    serial->write (b);
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.print(b, HEX); 
    REMOTEXY__DEBUGLOGS.print(' ');
#endif
  }
  
  uint8_t receiveByte () {
    uint8_t b = serial->read ();
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.print(b, HEX); 
    REMOTEXY__DEBUGLOGS.print(' ');
#endif
    return b;
  }
  
  
  uint8_t availableByte () {
    return serial->available ();
  };  


};


#if defined(REMOTEXY_PORT__HARDSERIAL)
#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, &REMOTEXY_SERIAL, REMOTEXY_SERIAL_SPEED)
#elif defined(REMOTEXY_PORT__SOFTSERIAL)
#define RemoteXY_Init() remotexy = new CRemoteXY (RemoteXY_CONF_PROGMEM, &RemoteXY, REMOTEXY_SERIAL_RX, REMOTEXY_SERIAL_TX, REMOTEXY_SERIAL_SPEED)
#endif

#endif //_REMOTEXY_MOD_SERIAL_H_
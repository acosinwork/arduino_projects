/* RemoteXY.h 
   A RemoteXY Library - Remote device control
   version 2.2.5   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
        
   Supported: 
    #define REMOTEXY_MODE__HARDSERIAL or old value REMOTEXY_MODE__HC05_HARDSERIAL, REMOTEXY_MODE__BLUETOOTH_SERIAL 
    #define REMOTEXY_MODE__SOFTSERIAL or old value REMOTEXY_MODE__HC05_SOFTSERIAL, REMOTEXY_MODE__BLUETOOTH_SOFTWARESERIAL 
    #define REMOTEXY_MODE__ETHERNET_LIB or old value REMOTEXY_MODE__W5100_SPI : ethernet library use
    #define REMOTEXY_MODE__WIFI_LIB : WiFi library use
    #define REMOTEXY_MODE__ESP8266POINT_HARDSERIAL 
    #define REMOTEXY_MODE__ESP8266POINT_SOFTSERIAL 
    #define REMOTEXY_MODE__ESP8266_HARDSERIAL 
    #define REMOTEXY_MODE__ESP8266_SOFTSERIAL 
    
   Only ESP8266 boards: WeMos
    #define REMOTEXY_MODE__WIFIPOINT_ESP8266LIB for ESP8266 boards: wemos
    #define REMOTEXY_MODE__WIFI_ESP8266LIB for ESP8266 boards: wemos

   Debug log info:
    #define REMOTEXY__DEBUGLOGS Serial

   ===========================================================
   version 2.2.5   
     - support MPIDE;
          
*/

#ifndef _REMOTEXY_H_
#define _REMOTEXY_H_

//#define REMOTEXY__DEBUGLOGS Serial
#define REMOTEXY__DEBUGLOGS_SPEED 115200


#if defined(REMOTEXY_MODE__HARDSERIAL) || defined(REMOTEXY_MODE__SERIAL) || defined(REMOTEXY_MODE__HC05_HARDSERIAL) 
  #define REMOTEXY_MOD__SERIAL
  #define REMOTEXY_PORT__HARDSERIAL
#elif defined(REMOTEXY_MODE__SOFTSERIAL) || defined(REMOTEXY_MODE__SOFTWARESERIAL) || defined(REMOTEXY_MODE__HC05_SOFTSERIAL)
  #define REMOTEXY_MOD__SERIAL
  #define REMOTEXY_PORT__SOFTSERIAL
#elif defined(REMOTEXY_MODE__ESP8266POINT_HARDSERIAL)
  #define REMOTEXY_MOD__ESP8266
  #define REMOTEXY_WIFI__POINT
  #define REMOTEXY_PORT__HARDSERIAL
#elif defined(REMOTEXY_MODE__ESP8266POINT_SOFTSERIAL)
  #define REMOTEXY_MOD__ESP8266
  #define REMOTEXY_WIFI__POINT
  #define REMOTEXY_PORT__SOFTSERIAL
#elif defined(REMOTEXY_MODE__ESP8266_HARDSERIAL)
  #define REMOTEXY_MOD__ESP8266
  #define REMOTEXY_PORT__HARDSERIAL
#elif defined(REMOTEXY_MODE__ESP8266_SOFTSERIAL)
  #define REMOTEXY_MOD__ESP8266
  #define REMOTEXY_PORT__SOFTSERIAL
#elif defined(REMOTEXY_MODE__ETHERNET_LIB) || defined(REMOTEXY_MODE__W5100_SPI)
  #define REMOTEXY_MOD__ETHERNET_LIB
#elif defined(REMOTEXY_MODE__WIFI_LIB)
  #define REMOTEXY_MOD__WIFI_LIB
#elif defined(REMOTEXY_MODE__ESP8266WIFIPOINT_LIB)
  #define REMOTEXY_MOD__ESP8266WIFI_LIB
  #define REMOTEXY_WIFI__POINT
#elif defined(REMOTEXY_MODE__ESP8266WIFI_LIB)
  #define REMOTEXY_MOD__ESP8266WIFI_LIB
#else
  #error RemoteXY mode does not defined or defined error: REMOTEXY_MODE__XXXXXXX 
#endif

#include <inttypes.h> 
#include "classes/RemoteXY_Lib.h"

/*
#if defined(REMOTEXY_PORT__HARDSERIAL) || defined(REMOTEXY__DEBUGLOGS)
  #include <HardwareSerial.h>
#endif 
*/


#if defined(REMOTEXY_MOD__SERIAL) 
  #include "modules/serial.h" 
#elif defined(REMOTEXY_MOD__ESP8266)
  #include "modules/esp8266.h" 
#elif defined(REMOTEXY_MOD__ETHERNET_LIB)
  #include "modules/ethernet.h" 
#elif defined(REMOTEXY_MOD__WIFI_LIB)
  #include "modules/wifi.h" 
#elif defined(REMOTEXY_MOD__ESP8266WIFI_LIB)
  #include "modules/esp8266wifi.h" 
#endif 


CRemoteXY *remotexy;

#define RemoteXY_Handler() remotexy->handler ()

#define RemoteXY_CONF const PROGMEM RemoteXY_CONF_PROGMEM


#endif //_REMOTEXY_H_


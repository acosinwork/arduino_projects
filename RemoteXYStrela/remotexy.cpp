#include "remotexy.h"
#include <arduino.h>


#define REMOTEXY_CONF_LENGTH   16
#define REMOTEXY_BUFFER_LENGTH 3
#define REMOTEXY_VALUES_LENGTH 3
#define REMOTEXY_OUTPUT_INDEX  2
#define REMOTEXY_TIMOUT  300
#define REMOTEXY_TIMOUT_DISCONNECTED  2000

typedef struct {  
  unsigned int count;
  unsigned int index;
  unsigned char command;
  unsigned char crc;
  unsigned long timeout;
} RemoteXY_State_TypeDef;

RemoteXY_State_TypeDef RemoteXY_State;
RemoteXY_TypeDef RemoteXY;
unsigned char RemoteXY_buffer[REMOTEXY_BUFFER_LENGTH];

unsigned char RemoteXY_conf[REMOTEXY_CONF_LENGTH] =
  { 1,7,5,0,0,0,63,63,2,66
  ,0,96,0,4,63,2 };


void RemoteXY_SerialInit (void) {  
  REMOTEXY_SERIAL.begin(9600);    
}
#define RemoteXY_SerialAvailable() REMOTEXY_SERIAL.available()
#define RemoteXY_SerialWrite(b)    REMOTEXY_SERIAL.write (b)
#define RemoteXY_SerialRead()      REMOTEXY_SERIAL.read()


void RemoteXY_Init (void) { 
  unsigned char i; 
  unsigned char* p = (unsigned char*)&RemoteXY; 
  for (i=0;i<REMOTEXY_VALUES_LENGTH;i++) *p++=0;      
  RemoteXY_State.index = 0; 
  RemoteXY_State.crc = 0;  
  RemoteXY_SerialInit (); 
  RemoteXY.connect_flag=0;
} 

void RemoteXY_WriteByte (unsigned char c, unsigned char * crc) { 
  RemoteXY_SerialWrite (c);  
  *crc-=c;   
} 

void RemoteXY_Send (unsigned char *p, unsigned int len) { 
  unsigned char c;   
  unsigned char crc = 0; 
  unsigned int i = len+4; 
   
  RemoteXY_WriteByte (i & 0xff, &crc); 
  RemoteXY_WriteByte ((i & 0xff00)>>8, &crc); 
  RemoteXY_WriteByte (RemoteXY_State.command, &crc);   
  while (len--) { 
    RemoteXY_WriteByte (*(p++), &crc); 
  }   
  RemoteXY_SerialWrite (crc); 
} 

void RemoteXY_Receive (unsigned char *p, unsigned int len) { 
  unsigned char *pi = RemoteXY_buffer; 
  while (len--) { 
    *p++=*pi++; 
  } 
} 

void RemoteXY_Handler (void) { 
  unsigned char c; 
  unsigned long tim; 
  while (RemoteXY_SerialAvailable() > 0) {   
    c = RemoteXY_SerialRead (); 
    RemoteXY_State.crc+=c; 
    switch (RemoteXY_State.index) { 
      case 0:  
        RemoteXY_State.count=c; 
        break; 
      case 1:  
        RemoteXY_State.count+=c<<8; 
        break; 
      case 2:  
        RemoteXY_State.command=c; 
        break; 
      default:  
        if (RemoteXY_State.index-3<REMOTEXY_BUFFER_LENGTH) 
          RemoteXY_buffer[RemoteXY_State.index-3]=c; 
    } 
    RemoteXY_State.index++; 
    RemoteXY_State.timeout=millis(); 
    if ((RemoteXY_State.index == RemoteXY_State.count) && (RemoteXY_State.count >= 4)) { 
      if (RemoteXY_State.crc == 0) { 
        switch (RemoteXY_State.command) { 
          case 0x00: 
            RemoteXY_Send (RemoteXY_conf, REMOTEXY_CONF_LENGTH); 
            RemoteXY.connect_flag=1;
            break;  
          case 0x40: 
            RemoteXY_Send ((unsigned char*)&RemoteXY, REMOTEXY_VALUES_LENGTH); 
            break;  
          case 0x80: 
            RemoteXY_Receive ((unsigned char*)&RemoteXY, RemoteXY_State.index-4);  
            RemoteXY_Send (0, 0); 
            break;  
          case 0xC0: 
            RemoteXY_Send ((unsigned char*)&RemoteXY+REMOTEXY_OUTPUT_INDEX,  
              REMOTEXY_VALUES_LENGTH-REMOTEXY_OUTPUT_INDEX); 
            break;  
        } 
      } 
      RemoteXY_State.index = 0; 
      RemoteXY_State.crc = 0;        
    } 
  } 
  if (RemoteXY_State.index>0) { 
    if (millis()-RemoteXY_State.timeout>REMOTEXY_TIMOUT) { 
      RemoteXY_State.index = 0; 
      RemoteXY_State.crc = 0;              
    } 
  } 
  if (millis()-RemoteXY_State.timeout>REMOTEXY_TIMOUT_DISCONNECTED) {
    RemoteXY.connect_flag=0; 
  } 
   
}

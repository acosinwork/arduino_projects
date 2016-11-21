#ifndef _REMOTEXY_API_H_
#define _REMOTEXY_API_H_

#include <inttypes.h> 
#include <stdlib.h>
#include <Arduino.h>


#define REMOTEXY_INPUT_LENGTH_INDEX  0
#define REMOTEXY_OUTPUT_LENGTH_INDEX  1 
#define REMOTEXY_CONF_LENGTH_INDEX 2 
#define REMOTEXY_CONF_INDEX 4 
#define REMOTEXY_PACKAGE_START_BYTE 0x55
#define REMOTEXY_TIMEOUT 5000


class CRemoteXY_API {
  protected:
  uint8_t *conf;
  uint8_t *var;
  uint8_t outputLength;
  uint8_t inputLength;
  uint16_t confLength;
  uint8_t *connect_flag;

  uint8_t *receiveBuffer;
  uint16_t receiveBufferLength;
  uint16_t receiveIndex;
  uint16_t receiveCRC;

  uint32_t wireTimeOut;
  
  uint8_t moduleRunning;
    
  protected:
  virtual uint8_t initModule () {return 1;};
  virtual void handlerModule () {};
  virtual void sendStart (uint16_t len) {};
  virtual void sendByte (uint8_t b) {};
  virtual uint8_t receiveByte () {};
  virtual uint8_t availableByte () {};  
  
  public:
  void init (const void * _conf, void * _var) {
    uint32_t ms;
    uint8_t i;
    uint8_t* p = (uint8_t*)_conf;
    inputLength = getConfByte (p+REMOTEXY_INPUT_LENGTH_INDEX);
    outputLength = getConfByte (p+REMOTEXY_OUTPUT_LENGTH_INDEX);
    confLength = getConfByte (p+REMOTEXY_CONF_LENGTH_INDEX) | 
                 (getConfByte (p+REMOTEXY_CONF_LENGTH_INDEX+1)<<8);
    conf = p+REMOTEXY_CONF_INDEX;
    var = (uint8_t*)_var;
    uint16_t varLength = outputLength+inputLength;
    connect_flag = var+varLength;
    *connect_flag = 0;
    
    receiveBufferLength=inputLength+6;
    receiveBuffer = (uint8_t*)malloc (receiveBufferLength);             
    
    p = var;
    i = varLength;
    while (i--) *p++=0;   
    receiveIndex=0;
    receiveCRC=initCRC ();
  
    wireTimeOut=0;
  
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.begin (REMOTEXY__DEBUGLOGS_SPEED);
#endif
  
    moduleRunning = initModule ();
#if defined(REMOTEXY__DEBUGLOGS)
    if (!moduleRunning) {
      REMOTEXY__DEBUGLOGS.write ("Module not available, RemoteXY stoped\r\n");
    }
#endif     
    
    
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.write("\r\n{ ");  
#endif
  }
  
  private:
  inline uint8_t getConfByte (uint8_t* p) {
    return pgm_read_byte_near (p);
  }
  
  public:
  void handler () {
    uint8_t *p, *kp;
    uint16_t i;  
    uint8_t b;
    uint16_t packageLength;
    
    if (!moduleRunning) return;
    
    handlerModule ();
    
    while (availableByte () > 0) {    
      b = receiveByte (); 
      
      if ((receiveIndex==0) && (b!=REMOTEXY_PACKAGE_START_BYTE)) continue;   
      receiveBuffer[receiveIndex++]=b;
      updateCRC (&receiveCRC, b);
      if (receiveIndex>receiveBufferLength) {       
        searchStartByte (1); //receiveBuffer overflow
      }
      while (1==1) {
        if (receiveIndex<6) return;
        packageLength = receiveBuffer[1]|(receiveBuffer[2]>>8);
        //packageLength=*(((uint16_t*)(receiveBuffer+1)));       
        if (packageLength>receiveBufferLength) searchStartByte (1); // error
        else if (packageLength<6) searchStartByte (1); // error
        else if (packageLength==receiveIndex) {      
          if (receiveCRC==0) {
            if (handleReceivePackage ()) {
              receiveIndex=0;
              receiveCRC=initCRC ();
              break;
            }
          }
          searchStartByte (1); // error 
        }
        else if (packageLength<receiveIndex) {
          uint16_t crc = initCRC ();
          p = receiveBuffer;
          i=packageLength;
          while (i--) updateCRC (&crc, *(p++)); 
          if (crc==0) {
            if (handleReceivePackage ()) {
              searchStartByte (packageLength);
              continue;
            }
          }
          searchStartByte (1);        
        }
        else break;
      }
    }  
    
    if (wireTimeOut<millis()) *connect_flag = 0;
  }
  
  private:
  uint16_t initCRC () {
    return 0xffff;
  }          
    
  private:
  void updateCRC (uint16_t *crc, uint8_t b) {
    *crc ^= b;
    for (uint8_t i=0; i<8; ++i) {
      if ((*crc) & 1) *crc = ((*crc) >> 1) ^ 0xA001;
      else *crc >>= 1;
    }
  } 
  
  private:  
  void sendByteUpdateCRC (uint8_t b, uint16_t *crc) {
    sendByte (b); 
    updateCRC (crc, b);
  }  

  private:  
  void sendPackage (uint8_t command, uint8_t *p, uint16_t length, uint8_t itConf) {
    uint16_t crc = initCRC ();
    uint16_t packageLength = length+6;
    sendStart (packageLength);
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.write("\r\n} ");  
#endif
    sendByteUpdateCRC (REMOTEXY_PACKAGE_START_BYTE, &crc);
    sendByteUpdateCRC (packageLength, &crc);
    sendByteUpdateCRC (packageLength>>8, &crc);
    //sendByteUpdateCRC (*((uint8_t*)(&packageLength)), &crc);
    //sendByteUpdateCRC (*((uint8_t*)(&packageLength)+1), &crc);
    sendByteUpdateCRC (command, &crc);  
    uint8_t b;
    while (length--) {
      if (itConf) b=getConfByte (p++);
      else b=*p++;
      sendByteUpdateCRC (b, &crc);
    }
    sendByte (crc);  
    sendByte (crc>>8);
    //sendByte (*(uint8_t*)(&crc));  
    //sendByte (*((uint8_t*)(&crc)+1));
#if defined(REMOTEXY__DEBUGLOGS)
    REMOTEXY__DEBUGLOGS.write("\r\n{ ");  
#endif 
  }
  
  private:  
  void searchStartByte (uint16_t pos) {
    uint8_t *p, *kp;
    uint16_t i;
    uint16_t ri = receiveIndex;
    p=receiveBuffer+pos;
    receiveCRC=initCRC ();
    for (i=pos; i<ri; i++) {
      if (*p==REMOTEXY_PACKAGE_START_BYTE) {      
        kp=receiveBuffer;
        receiveIndex=receiveIndex-i;
        while (i++<ri) {
          updateCRC (&receiveCRC, *p);              
          *(kp++)=*(p++);
        }
        return;
      }
      p++;
    }        
    receiveIndex=0;
  }  
  
  uint8_t handleReceivePackage () {
    uint8_t command, i;
    uint16_t length;
    uint8_t *p, *kp;
       
    length = receiveBuffer[1]|(receiveBuffer[2]>>8); 
    length-=6;
    //length=*(uint16_t*)(receiveBuffer+1)-6;
    command = receiveBuffer[3];
    switch (command) {  
      case 0x00:  
        sendPackage (command, conf, confLength,  1);
        break;   
      case 0x40:  
        sendPackage (command, var, inputLength+outputLength, 0);
        break;   
      case 0x80:  
        if (length==inputLength) {
          p=receiveBuffer+4;
          kp=var;
          i= inputLength;
          while (i--) *kp++=*p++;
        }
        sendPackage (command, 0, 0, 0);
        break;   
      case 0xC0:  
        sendPackage (command, var+inputLength, outputLength, 0);
        break;   
      default:
        return 0; 
    }  
    *connect_flag = 1;
    wireTimeOut=millis()+REMOTEXY_TIMEOUT;  
    return 1;
  }
    
};

#endif //_REMOTEXY_API_H_


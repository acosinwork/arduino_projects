// Файл, автоматически сгенерированный online-конструктором
// форм для смартфона remoteXY
// http://remotexy.com/en/editor/a1cf60078a90eb8434b694227961932b/

////////////////////////////////////////////// 
//        RemoteXY include library          // 
//     use  library  version 2.2.5 or up    // 
//   use ANDROID app version 3.7.1 or up    // 
////////////////////////////////////////////// 

/* RemoteXY select connection mode and include library */ 
#define REMOTEXY_MODE__HC05_HARDSERIAL

#include <RemoteXY.h> 

/* RemoteXY connection settings */ 
#define REMOTEXY_SERIAL Serial1 
#define REMOTEXY_SERIAL_SPEED 9600 


/* RemoteXY configurate  */ 
#pragma pack(push, 1) 
uint8_t RemoteXY_CONF[] = 
  { 2,1,26,0,6,5,0,3,133,1
  ,10,98,11,2,1,0,63,40,12,12
  ,1,82,0,65,4,25,40,12,12,2
   }; 
   
/* this structure defines all the variables of your control interface */ 
struct { 

    /* input variable */
  uint8_t buttons; /* =0 if select position A, =1 if position B, =2 if position C, ... */
  uint8_t record; /* =1 if button pressed, else =0 */

    /* output variable */
  uint8_t recMode_r; /* =0..255 LED Red brightness */

    /* other variable */
  uint8_t connect_flag;  /* =1 if wire connected, else =0 */

} RemoteXY; 
#pragma pack(pop) 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 


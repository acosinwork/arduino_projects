/* RemoteXY select connection mode and include library */

#define REMOTEXY_MODE__HC05_HARDSERIAL

#include <IRremote.h>
#include <EEPROM.h>
#include <RemoteXY.h>

/* RemoteXY connection settings */
#define REMOTEXY_SERIAL Serial1
#define REMOTEXY_SERIAL_SPEED 9600

/* RemoteXY configurate  */
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
{ 3, 1, 33, 0, 6, 5, 0, 3, 138, 1
  , 10, 98, 11, 2, 3, 138, 1, 42, 98, 11
  , 2, 1, 0, 77, 25, 12, 12, 1, 82, 0
  , 65, 4, 59, 25, 12, 12, 2
};

/* this structure defines all the variables of your control interface */
struct {

  /* input variable */
  uint8_t buttons; /* =0 if select position A, =1 if position B, =2 if position C, ... */
  uint8_t macros; /* =0 if select position A, =1 if position B, =2 if position C, ... */
  uint8_t record; /* =1 if button pressed, else =0 */

  /* output variable */
  uint8_t recMode_r; /* =0..255 LED Red brightness */

  /* other variable */
  uint8_t connect_flag;  /* =1 if wire connected, else =0 */

} RemoteXY;
#pragma pack(pop)

#define IR_BUTTONS 10
#define IR_MACROS 10

bool recordState = false;
bool recButtonLastState = false;

int RECV_PIN = 3;
/*
int BUTTON_PIN = 2;
int STATUS_PIN = 8;
*/
int SEND_PIN = 9;

byte NO_DATA = 255;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

struct irData {
  int codeType = -1; // The type of code
  unsigned long codeValue = 0; // The code value if not raw
  int codeLen = 0; // The length of the code
  int toggle = 0; // The RC5/6 toggle state
};

irData irSygnals[IR_BUTTONS];

byte macrosData[IR_MACROS][IR_BUTTONS];
byte currentMacro[IR_BUTTONS];
byte macroPos = 0;

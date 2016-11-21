
//////////////////////////////////////////////
//        RemoteXY include library          //
//     use  library  version 2.2.5 or up    //
//   use ANDROID app version 3.7.1 or up    //
//////////////////////////////////////////////

/* RemoteXY select connection mode and include library */
#define REMOTEXY_MODE__HC05_HARDSERIAL

#include <RemoteXY.h>

#include <IRremote.h>

#include <EEPROM.h>

//#include "irReadWrite.ino"

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

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_RECORD 8
#define IR_BUTTONS 10
#define IR_MACROS 10

bool recordState = false;
bool recButtonLastState = false;

int RECV_PIN = 3;
int BUTTON_PIN = 2;
int STATUS_PIN = 8;
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

//decode_results

void setup()
{
  RemoteXY_Init ();

  irrecv.enableIRIn(); // Start the receiver
  pinMode(BUTTON_PIN, INPUT);
  pinMode(STATUS_PIN, OUTPUT);
  pinMode (PIN_RECORD, OUTPUT);

  for (int i = 0; i < IR_MACROS; i++) {
    currentMacro[i] = NO_DATA;
    for (int y = 0; y < IR_BUTTONS; y++)
      macrosData[i][y] = NO_DATA;
  }

  EEPROM.get(0, irSygnals);


  EEPROM.get(sizeof(irSygnals), macrosData);

  //  irrecv.enableIRIn(); // Start the receiver

  // TODO you setup code

}

/*
  // Storage for the recorded code
  int codeType = -1; // The type of code
  unsigned long codeValue; // The code value if not raw
  unsigned int rawCodes[RAWBUF]; // The durations if raw
  int codeLen; // The length of the code
  int toggle = 0; // The RC5/6 toggle state
*/

// Stores the code for later playback
// Most of this code is just logging
void storeCode(decode_results *results, int buttonNumber) {
  irSygnals[buttonNumber].codeType = results->decode_type;
  int count = results->rawlen;
  if (irSygnals[buttonNumber].codeType == UNKNOWN) {
    /*
        Serial.println("Received unknown code, saving as raw");
        codeLen = results->rawlen - 1;
        // To store raw codes:
        // Drop first value (gap)
        // Convert from ticks to microseconds
        // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
        for (int i = 1; i <= codeLen; i++) {
          if (i % 2) {
            // Mark
            rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
            Serial.print(" m");
          }
          else {
            // Space
            rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
            Serial.print(" s");
          }
          Serial.print(rawCodes[i - 1], DEC);
        }
        Serial.println("");
      }
    */
  }  else {
    if (irSygnals[buttonNumber].codeType == NEC) {
      Serial.print("Received NEC: ");
      if (results->value == REPEAT) {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    }
    else if (irSygnals[buttonNumber].codeType == SONY) {
      Serial.print("Received SONY: ");
    }
    else if (irSygnals[buttonNumber].codeType == PANASONIC) {
      Serial.print("Received PANASONIC: ");
    }
    else if (irSygnals[buttonNumber].codeType == JVC) {
      Serial.print("Received JVC: ");
    }
    else if (irSygnals[buttonNumber].codeType == RC5) {
      Serial.print("Received RC5: ");
    }
    else if (irSygnals[buttonNumber].codeType == RC6) {
      Serial.print("Received RC6: ");

    }
    else {
      Serial.print("Unexpected codeType ");
      Serial.print(irSygnals[buttonNumber].codeType, DEC);
      Serial.println("");
    }
    Serial.println(results->value, HEX);
    irSygnals[buttonNumber].codeValue = results->value;
    irSygnals[buttonNumber].codeLen = results->bits;
  }
}

void sendCode(int repeat, int buttonNumber) {
  if (irSygnals[buttonNumber].codeType == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, irSygnals[buttonNumber].codeLen);
      Serial.println("Sent NEC repeat");
    }
    else {
      irsend.sendNEC(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
      Serial.print("Sent NEC ");
      Serial.println(irSygnals[buttonNumber].codeValue, HEX);
    }
  }
  else if (irSygnals[buttonNumber].codeType == SONY) {
    irsend.sendSony(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
    Serial.print("Sent Sony ");
    Serial.println(irSygnals[buttonNumber].codeValue, HEX);
  }
  else if (irSygnals[buttonNumber].codeType == PANASONIC) {
    irsend.sendPanasonic(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
    Serial.print("Sent Panasonic");
    Serial.println(irSygnals[buttonNumber].codeValue, HEX);
  }
  else if (irSygnals[buttonNumber].codeType == JVC) {
    irsend.sendPanasonic(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
    Serial.print("Sent JVC");
    Serial.println(irSygnals[buttonNumber].codeValue, HEX);
  }
  else if (irSygnals[buttonNumber].codeType == RC5 || irSygnals[buttonNumber].codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      irSygnals[buttonNumber].toggle = 1 - irSygnals[buttonNumber].toggle;
    }
    // Put the toggle bit into the code to send
    irSygnals[buttonNumber].codeValue = irSygnals[buttonNumber].codeValue & ~(1 << (irSygnals[buttonNumber].codeLen - 1));
    irSygnals[buttonNumber].codeValue = irSygnals[buttonNumber].codeValue | (irSygnals[buttonNumber].toggle << (irSygnals[buttonNumber].codeLen - 1));
    if (irSygnals[buttonNumber].codeType == RC5) {
      Serial.print("Sent RC5 ");
      Serial.println(irSygnals[buttonNumber].codeValue, HEX);
      irsend.sendRC5(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
    }
    else {
      irsend.sendRC6(irSygnals[buttonNumber].codeValue, irSygnals[buttonNumber].codeLen);
      Serial.print("Sent RC6 ");
      Serial.println(irSygnals[buttonNumber].codeValue, HEX);
    }
  }
  else if (irSygnals[buttonNumber].codeType == UNKNOWN /* i.e. raw */) {
    /*  // Assume 38 KHz
      irsend.sendRaw(rawCodes, irSygnals[buttonNumber].codeLen, 38);
      Serial.println("Sent raw");
    */
  }
}

byte macroLast = 0;
byte buttonLast = 0;

void sendButton(byte button)
{
  if (irSygnals[button].codeType != -1)
    sendCode(false, button);
}
void sendMacro(byte macro)
{
  int i = 0;
  while ((i != NO_DATA) && (i < IR_BUTTONS)) {
    sendButton(macrosData[macro][i]);
    ++i;
    delay(50);
  }
}

void saveAll()
{

  EEPROM.put(0, irSygnals);

  EEPROM.put(sizeof(irSygnals), macrosData);
}

void loop()
{
  RemoteXY_Handler ();

  bool recButtonState = RemoteXY.record;
  if (recButtonLastState != recButtonState) {
    if (recButtonState) {
      recordState = !recordState;
    }

    if (!recordState) {
      saveAll();
      macroPos = 0;
      for (int i = 0; i < IR_BUTTONS; i++) {
        currentMacro[i] = NO_DATA;
      }
    }

    RemoteXY.recMode_r = 255 * recordState;
    recButtonLastState = recButtonState;
  }

  if (RemoteXY.buttons != buttonLast) {
    buttonLast = RemoteXY.buttons;

    if (!recordState) {
      sendButton(buttonLast);
      /*      if (irSygnals[buttonLast].codeType != -1)
            {
              sendCode(false, buttonLast);
        //       irrecv.enableIRIn();
      */

    } else if (irrecv.decode(&results)) {
      digitalWrite(STATUS_PIN, HIGH);
      storeCode(&results, buttonLast);
      irrecv.resume(); // resume receiver
      digitalWrite(STATUS_PIN, LOW);
    } else if (macroPos < IR_BUTTONS) {
      currentMacro[macroPos] = buttonLast;
      ++macroPos;
    }
  }

  if (RemoteXY.macros != macroLast) {
    macroLast = RemoteXY.macros;
    if (!recordState) {
      sendMacro(macroLast);
    } else {
      for (int i = 0; i < IR_BUTTONS; i++) {
        macrosData[macroLast][i] = currentMacro[i];
      }
    }

    //  digitalWrite(PIN_RECORD, (RemoteXY.record == 0) ? LOW : HIGH);

    // TODO you loop code
    // use the RemoteXY structure for data transfer
  }
}
/*
  void loop() {
  // If button pressed, send the code.
  int buttonState = !digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("Released");
    irrecv.enableIRIn(); // Re-enable receiver
  }

  if (buttonState) {
    Serial.println("Pressed, sending");
    digitalWrite(STATUS_PIN, HIGH);
    sendCode(lastButtonState == buttonState);
    digitalWrite(STATUS_PIN, LOW);
    delay(50); // Wait a bit between retransmissions
  }
  else if (irrecv.decode(&results)) {
    digitalWrite(STATUS_PIN, HIGH);
    storeCode(&results);
    irrecv.resume(); // resume receiver
    digitalWrite(STATUS_PIN, LOW);
  }
  lastButtonState = buttonState;
  }
*/



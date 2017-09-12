#include "headers.h"

void setup()
{
  RemoteXY_Init ();  

  irrecv.enableIRIn(); // Start the receiver

   for (int i = 0; i < IR_MACROS; i++) {
    currentMacro[i] = NO_DATA;
    for (int y = 0; y < IR_BUTTONS; y++)
      macrosData[i][y] = NO_DATA;
  }
  EEPROM.get(0, irSygnals);
  EEPROM.get(sizeof(irSygnals), macrosData);
}

// Stores the code for later playback
// Most of this code is just logging
void storeCode(decode_results *results, int buttonNumber) {
  if ((results->decode_type != UNKNOWN) && (results->value != REPEAT)) {
    irSygnals[buttonNumber].codeType = results->decode_type;
    irSygnals[buttonNumber].codeValue = results->value;
    irSygnals[buttonNumber].codeLen = results->bits;
  }
}

void sendCode(int repeat, int buttonNumber) {
  int codeType = irSygnals[buttonNumber].codeType;
  unsigned long codeValue = irSygnals[buttonNumber].codeValue;
  int codeLen = irSygnals[buttonNumber].codeLen;

  if (codeType == RC5 || codeType == RC6) {
    // Flip the toggle bit for a new button press
    int toggle = irSygnals[buttonNumber].toggle ^= 1;
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (codeLen - 1));
    codeValue = codeValue | (toggle << (codeLen - 1));
    if (codeType == RC5) {
      irsend.sendRC5(codeValue, codeLen);
    } else {
      irsend.sendRC6(codeValue, codeLen);
    }
    return;
  }
  switch (codeType)
  {
    case NEC:
      irsend.sendNEC(codeValue, codeLen);
      break;
    case SONY:
      irsend.sendSony(codeValue, codeLen);
      break;
    case PANASONIC:
      irsend.sendPanasonic(codeValue, codeLen);
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
    delay(500);
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
    } else if (irrecv.decode(&results)) {
      storeCode(&results, buttonLast);
      irrecv.resume(); // resume receiver
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
  }
}


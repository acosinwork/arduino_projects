
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523


/*Simon Says game. Now with sound effects.

Originaly made by Robert Spann
Code trimmed and sound effects added by digimike

Buttons are to be set on there designated pins without pull down resistors
and connected to ground rather then +5.
*/
#include <QuadDisplay.h>
#define BUZZER_PIN          X3
#define ANALOG_PIN_FOR_RND  A2
#define DISPLAY_PIN         Y4

#define BUTTON_1            X1
#define BUTTON_2            X4
#define BUTTON_3            A1
#define BUTTON_4            A4

#define LED_1               Y2
#define LED_2               Y3
#define LED_3               Z2
#define LED_4               Z3

int startTune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int duration[] = {100, 100, 100, 300, 100, 300};
int ledBeep[] = {NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4};

byte button[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4}; //The four button input pins
byte ledPin[] = {LED_1, LED_2, LED_3, LED_4};

int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];

void allLedWrite(boolean state)
{
  for (int x = 0; x < 4; x++) // LED pins are outputs
    digitalWrite(ledPin[x], state);
}

void showLed(byte currentLed)
{
  digitalWrite(ledPin[currentLed], HIGH);
  tone(BUZZER_PIN, ledBeep[currentLed], 100); //speakerpin.play(NOTE_G3, 100);
  delay(400);
  digitalWrite(ledPin[currentLed], LOW);
  delay(100);
}

boolean buttonIsPressed(byte currentButton, int currentTurn)
{
  boolean result = false;
  if (!digitalRead(button[currentButton]))
  { //Checking for button push

    digitalWrite(ledPin[currentButton], HIGH);
    tone(BUZZER_PIN, ledBeep[currentButton], 100); //speakerpin.play(NOTE_G3, 100);
    delay(200);
    digitalWrite(ledPin[currentButton], LOW);
    inputArray[currentTurn] = currentButton;
    delay(250);
    Serial.print(" ");
    Serial.print(currentButton);
    if (inputArray[currentTurn] != randomArray[currentTurn]) { //Checks value input by user and checks it against
      fail();                              //the value in the same spot on the generated array
    }                                      //The fail function is called if it does not match
    result = true;
    //    userChoice=true;
  }
  return result;
}


void setup()
{
  Serial.begin(9600);
  //speakerpin.begin(BUZZER_PIN);

  for (int x = 0; x < 4; x++) // LED pins are outputs
  {
    pinMode(ledPin[x], OUTPUT);
    pinMode(button[x], INPUT);  // button pins are inputs
  }

  randomSeed(analogRead(ANALOG_PIN_FOR_RND)); //Added to generate "more randomness" with the randomArray for the output function
  for (int thisNote = 0; thisNote < 13; thisNote ++) {
    // play the next note:
    //speakerpin.play(startTune[thisNote]);
    tone(BUZZER_PIN, startTune[thisNote], duration2[thisNote]);
    // hold the note:
    if (thisNote == 0 || thisNote == 2 || thisNote == 4 || thisNote == 6)
    {
      digitalWrite(ledPin[0], HIGH);
    }
    if (thisNote == 1 || thisNote == 3 || thisNote == 5 || thisNote == 7 || thisNote == 9 || thisNote == 11)
    {
      digitalWrite(ledPin[1], HIGH);
    }
    if (thisNote == 8 || thisNote == 12)
    {
      digitalWrite(ledPin[2], HIGH);
    }
    if (thisNote == 10)
    {
      digitalWrite(ledPin[3], HIGH);
    }
    delay(duration2[thisNote]);
    // stop for the next note:
    //speakerpin.stop();

    allLedWrite(LOW);

    delay(25);
  }
  delay(1000);


}

void loop()
{

  for (int y = 0; y <= 99; y++)
  {
    //function for generating the array to be matched by the player
    displayInt(DISPLAY_PIN, turn);
    allLedWrite(HIGH);

    for (int thisNote = 0; thisNote < 6; thisNote ++) {
      // play the next note:
      tone(BUZZER_PIN, note[thisNote], duration[thisNote]);//speakerpin.play(note[thisNote]);
      // hold the note:
      delay(duration[thisNote]);
      delay(25);
    }

    allLedWrite(LOW);

    delay(1000);

    for (int y = turn; y <= turn; y++)
    { //Limited by the turn variable
      Serial.println(""); //Some serial output to follow along
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      randomArray[y] = random(0, 4); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
      for (int x = 0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);

        showLed(randomArray[x]);

      }
    }
    input();

  }
}



void input() { //Function for allowing user input and checking input against the generated array

  for (int x = 0; x <= turn;)
  { //Statement controlled by turn count

    for (int y = 0; y < 4; y++)
    {

      if (buttonIsPressed(y, x))
        ++x;
    }
  }

  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again

}

void fail() { //Function used if the player fails to match the sequence

  for (int y = 0; y <= 2; y++)
  { //Flashes lights for failure

    allLedWrite(HIGH);

    tone(BUZZER_PIN, NOTE_G3, 300);
    delay(200);

    allLedWrite(LOW);

    tone(BUZZER_PIN, NOTE_C3, 300);
    delay(200);
  }
  delay(500);
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}



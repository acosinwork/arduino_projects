/*Simon Says game. Now with sound effects. 
 
 Originaly made by Robert Spann
 Code trimmed and sound effects added by digimike
 
 Buttons are to be set on there designated pins without pull down resistors
 and connected to ground rather then +5. 
 */

//Display

const byte PIN_CTL = A5;

int level=0;

const byte numbers[] = {
  0b00000011,  //0
  0b10011111,  //1
  0b00100101,  //2
  0b00001101,  //3
  0b10011001,  //4
  0b01001001,  //5
  0b01000001,  //6
  0b00011111,  //7
  0b00000001,  //8
  0b00001001   //9
};

void sendByte(byte data, const byte n = 8)
{
  for (byte i = n; i > 0; i--)
  {
    if (data & 1)
    {
      noInterrupts();
      digitalWrite(PIN_CTL, LOW);
      digitalWrite(PIN_CTL, HIGH);
      interrupts();
      delayMicroseconds(30);
    }
    else
    {
      digitalWrite(PIN_CTL, LOW);
      delayMicroseconds(15);
      digitalWrite(PIN_CTL, HIGH);
      delayMicroseconds(60);
    }
    data >>= 1;
  }
}

void latch()
{
  digitalWrite(PIN_CTL, LOW);
  delayMicroseconds(100);
  digitalWrite(PIN_CTL, HIGH);
  delayMicroseconds(300);
}

void clearDisplay()
{
  for (byte y = 0; y<4; ++y){
    sendByte(0b11111111);
  }
  latch();
}

void displayInt(int int_data)
{
  byte displayValue[] = {
    0b11111111,
    0b11111111, 
    0b11111111, 
    0b11111111  
  };
  byte capacity;

  int pure_data=int_data;

  if ((pure_data>999)&&(pure_data<10000)) 
  {

    capacity=int_data/1000;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[0]=numbers[capacity];
      int_data=int_data - (capacity*1000);
    }
  }
  if (pure_data>99) 
  {
    capacity=int_data/100;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[1]=numbers[capacity];
      int_data=int_data - (capacity*100);
    }
  }
  if (pure_data>9) 
  {
    capacity=int_data/10;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[2]=numbers[capacity];
      int_data=int_data - (capacity*10);
    }
  }
  capacity=int_data;  

  if ((capacity >= 0)&&(capacity <10))
  {
    displayValue[3]=numbers[capacity];
  }


  for (capacity=0; capacity<4; ++capacity)
  {
    sendByte(displayValue[capacity]);
  }
  latch();
}

// Simon


#include <Tone.h>
Tone speakerpin;
int starttune[] = {
  NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {
  100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int note[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int duration[] = {
  100, 100, 100, 300, 100, 300};
byte button[] = {
  12, 7, 2, 4}; //The four button input pins
byte ledpin[] = {
  5, 6, 9, 10};  // LED pins
int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];  

int brightness;


void setup() 
{

  //display
  pinMode(PIN_CTL, OUTPUT);
  digitalWrite(PIN_CTL, HIGH);
  clearDisplay();


  //simon
  Serial.begin(9600);
  speakerpin.begin(3); // speaker is on pin 13

    for(int x=0; x<4; x++)  // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }

  for(int x=0; x<4; x++) 
  {
    pinMode(button[x], INPUT);  // button pins are inputs
    //digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(A1)); //Added to generate "more randomness" with the randomArray for the output function
  for (int thisNote = 0; thisNote < 13; thisNote ++) {
    // play the next note:
    speakerpin.play(starttune[thisNote]);
    // hold the note:
    if (thisNote==0 || thisNote==2 || thisNote==4 || thisNote== 6)
    {
      digitalWrite(ledpin[0], HIGH);
    }
    if (thisNote==1 || thisNote==3 || thisNote==5 || thisNote== 7 || thisNote==9 || thisNote==11)
    {
      digitalWrite(ledpin[1], HIGH);
    }
    if (thisNote==8 || thisNote==12)
    {
      digitalWrite(ledpin[2], HIGH);
    }  
    if (thisNote==10)
    {   
      digitalWrite(ledpin[3], HIGH);
    }
    delay(duration2[thisNote]);
    // stop for the next note:
    speakerpin.stop();
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    delay(25);
  }
  delay(1000);


}

void loop() 
{   

  for (int y=0; y<=99; y++)
  {
    displayInt(level++);
    brightness = analogRead(A0)/4;
    //function for generating the array to be matched by the player
    analogWrite(ledpin[0], brightness);
    analogWrite(ledpin[1], brightness);
    analogWrite(ledpin[2], brightness);
    analogWrite(ledpin[3], brightness);

    for (int thisNote = 0; thisNote < 6; thisNote ++) {
      // play the next note:
      speakerpin.play(note[thisNote]);
      // hold the note:
      delay(duration[thisNote]);
      // stop for the next note:
      speakerpin.stop();
      delay(25);
    }

    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);

    delay(1000);

    for (int y=turn; y <= turn; y++)
    { //Limited by the turn variable
      Serial.println(""); //Some serial output to follow along
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      randomArray[y] = random(1, 5); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
      for (int x=0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);

        for(int y=0; y<4; y++)
        {

          if (randomArray[x] == 1 && ledpin[y] == 5) 
          {  //if statements to display the stored values in the array
            analogWrite(ledpin[y], brightness);
            speakerpin.play(NOTE_G3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 2 && ledpin[y] == 6) 
          {
            analogWrite(ledpin[y], brightness);
            speakerpin.play(NOTE_A3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 3 && ledpin[y] == 9) 
          {
            analogWrite(ledpin[y], brightness);
            speakerpin.play(NOTE_B3, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 4 && ledpin[y] == 10) 
          {
            analogWrite(ledpin[y], brightness);
            speakerpin.play(NOTE_C4, 100);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }
        }
      }
    }
    input();

  }
}



void input() { //Function for allowing user input and checking input against the generated array

  brightness = analogRead(A0)/4;

  //boolean userChoice=false;

  //while (!userChoice) {

  for (int x=0; x <= turn;)
  { //Statement controlled by turn count

    for(int y=0; y<4; y++)
    {

      buttonstate = digitalRead(button[y]);

      if (buttonstate == LOW && button[y] == 12)
      { //Checking for button push
        analogWrite(ledpin[0], brightness);
        speakerpin.play(NOTE_G3, 100);
        delay(200);
        digitalWrite(ledpin[0], LOW);
        inputArray[x] = 1;
        delay(250);
        Serial.print(" ");
        Serial.print(1);
        if (inputArray[x] != randomArray[x]) { //Checks value input by user and checks it against
          fail();                              //the value in the same spot on the generated array
        }                                      //The fail function is called if it does not match
        x++;
        //    userChoice=true;
      }
      if (buttonstate == LOW && button[y] == 7)
      {
        analogWrite(ledpin[1], brightness);
        speakerpin.play(NOTE_A3, 100);
        delay(200);
        digitalWrite(ledpin[1], LOW);
        inputArray[x] = 2;
        delay(250);
        Serial.print(" ");
        Serial.print(2);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
        //           userChoice=true;
      }

      if (buttonstate == LOW && button[y] == 2)
      {
        analogWrite(ledpin[2], brightness);
        speakerpin.play(NOTE_B3, 100);
        delay(200);
        digitalWrite(ledpin[2], LOW);
        inputArray[x] = 3;
        delay(250);
        Serial.print(" ");
        Serial.print(3);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
        //           userChoice=true;
      }

      if (buttonstate == LOW && button[y] == 4)
      {
        analogWrite(ledpin[3], brightness);
        speakerpin.play(NOTE_C4, 100);
        delay(200);
        digitalWrite(ledpin[3], LOW);
        inputArray[x] = 4;
        delay(250);
        Serial.print(" ");
        Serial.print(4);
        if (inputArray[x] != randomArray[x]) 
        {
          fail();
        }
        x++;
        //             userChoice=true;
      }
    }
  }

  //}
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
}

void fail() { //Function used if the player fails to match the sequence
 
  level=0;
  
  for (int y=0; y<=2; y++)
  { //Flashes lights for failure

    analogWrite(ledpin[0], brightness);
    analogWrite(ledpin[1], brightness);
    analogWrite(ledpin[2], brightness);
    analogWrite(ledpin[3], brightness);
    speakerpin.play(NOTE_G3, 300);
    delay(200);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    speakerpin.play(NOTE_C3, 300);
    delay(200);
  }
  delay(500);
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}





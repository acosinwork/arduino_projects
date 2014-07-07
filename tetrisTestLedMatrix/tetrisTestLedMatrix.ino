unsigned char latchPin = 4;
unsigned char clockPin = 3;
unsigned char dataPin = 2;
unsigned char rowPin = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);


  int i;
  for (i = 0; i < 8; i++)
    pinMode(rowPin + i, OUTPUT);

//    pinMode(10, OUTPUT);

}

void loop() {

//  for (int i = 6; i < 14; ++i)
//  {
    digitalWrite(latchPin, LOW);

    shiftOut(dataPin, clockPin, LSBFIRST, 0);
    shiftOut(dataPin, clockPin, LSBFIRST, 0);


    digitalWrite(latchPin, HIGH);

  //  digitalWrite(i - 1, LOW);

    digitalWrite(13, HIGH);
    delay(2000);
//  }
//  digitalWrite(13, LOW);

}

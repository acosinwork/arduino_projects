/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// int pause=5;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  //  Serial.begin(9600);
  pinMode(5, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  for (int pause = 0; pause < 6; ++pause)
  {
    for (int i = 0; i < 256; ++i)
    {
      analogWrite(5, i);
      delay(pause);
    }
    for (int i = 0; i < 256; ++i)
    {
      analogWrite(5, 255 - i);
      delay(pause);
    }
  }
    for (int pause = 0; pause < 6; ++pause)
  {
    for (int i = 0; i < 256; ++i)
    {
      analogWrite(5, i);
      delay(5-pause);
    }
    for (int i = 0; i < 256; ++i)
    {
      analogWrite(5, 255 - i);
      delay(5-pause);
    }
  }
}

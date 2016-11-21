int dir = 3;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
pinMode(dir, OUTPUT);

  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600);

}

void loop() // run over and over
{
  if (Serial1.available())
    Serial.write(Serial1.read());
  if (Serial.available())
    send(Serial.read());
}

void send(uint8_t a)
{
    digitalWrite(dir, HIGH);
    delay(10);
    Serial1.print(a);
    delay(10);
    digitalWrite(dir, LOW);  
}


int dir = 3;
uint8_t tim = 0;
unsigned long currentTime = 0;
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(dir, OUTPUT);
}

void loop() // run over and over
{

  if (millis() - currentTime > 1000) {
    while (Serial.available())
      send(Serial.read());
    currentTime = millis();
  }
}

void send(uint8_t a)
{
  digitalWrite(dir, HIGH);
  delay(10);
  Serial.write(a);
  delay(10);
  digitalWrite(dir, LOW);
}

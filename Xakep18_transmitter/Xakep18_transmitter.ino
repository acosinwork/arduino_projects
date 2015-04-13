#define TRANSMIT_PIN Y1
#define SEND_BIT_INTERVAL 50

void setup() {
  pinMode(TRANSMIT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  while (Serial.available())
    transmit(Serial.read());
}

void transmit(char message)
{
  digitalWrite(TRANSMIT_PIN, HIGH);
  for (int i = 0; i < 8; ++i)
  {
    delay(SEND_BIT_INTERVAL);
    digitalWrite(TRANSMIT_PIN, bitRead(message, i));
  }
  digitalWrite(TRANSMIT_PIN, LOW);
  delay(SEND_BIT_INTERVAL * 2);
}


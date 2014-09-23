#define RECIEVE_PIN  A2
#define TRANSMIT_PIN A3
#define POT_PIN A4
#define SEND_BIT_INTERVAL 30

int treshold;

void setup() {
  pinMode(TRANSMIT_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  treshold = analogRead(POT_PIN);

  if (analogRead(RECIEVE_PIN) < treshold)
    recieve();

  while (Serial.available() > 0)
    transmit(Serial.read());
}

void recieve()
{
  char message = 0;
  delay(SEND_BIT_INTERVAL);
  for (int i = 0; i < 8; ++i)
  {
    delay(SEND_BIT_INTERVAL / 2);
    if (analogRead(RECIEVE_PIN) < treshold)
      bitSet(message, i);
    else
      bitClear(message, i);
    delay(SEND_BIT_INTERVAL / 2);
  }
  Serial.print(message);
}

void transmit(char message)
{
  digitalWrite(TRANSMIT_PIN, HIGH);
  for (int i = 0; i < 8; ++i)
  {
    delay(SEND_BIT_INTERVAL);
    digitalWrite(TRANSMIT_PIN, bitRead(message, i));
  }

  delay(SEND_BIT_INTERVAL * 2);
}


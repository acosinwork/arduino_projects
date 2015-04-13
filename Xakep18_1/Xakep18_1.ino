#define RECIEVE_PIN  A2
#define TRANSMIT_PIN A3
#define SEND_BIT_INTERVAL 50

int treshold;

void setup() {
  pinMode(TRANSMIT_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
    treshold = 0.8 * analogRead(RECIEVE_PIN);
}

void loop() {
  if (analogRead(RECIEVE_PIN) < treshold)
    Serial.print(recieve());

  while (Serial.available() > 0)
    transmit(Serial.read());
}

char recieve()
{
  char message = 0;
  delay(SEND_BIT_INTERVAL * 1.5);
  for (int i = 0; i < 8; ++i)
  {
    message |= (analogRead(RECIEVE_PIN) < treshold) << i;
    delay(SEND_BIT_INTERVAL);
  }
  return message;
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



#define RECIEVE_PIN  A2
#define TRANSMIT_PIN A3
#define POT_PIN A3
#define SEND_BIT_INTERVAL 50

//bool transmiting = false;
int treshold;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(TRANSMIT_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  treshold = analogRead(POT_PIN);

  if (analogRead(RECIEVE_PIN) < treshold)
  {
    delay(SEND_BIT_INTERVAL);
    recieve();
  }

  while (Serial.available() > 0)
  {
    transmit(Serial.read());
  }
}

void recieve()
{
  char message = 0;
  for (int i = 0; i < 8; ++i)
  {
    delay(SEND_BIT_INTERVAL / 2);
    if (analogRead(RECIEVE_PIN) < treshold)
      bitSet(message, i);
    else
      bitClear(message, i);
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
}

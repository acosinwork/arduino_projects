

void setup() {
  // put your setup code here, to run once:
  //  for (int i = 1; i < 13; ++i)
  //    pinMode(i, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //  for (int i = 1; i < 13; ++i)
  //    digitalWrite(i, !digitalRead(i));
  Serial.println("BEGIN");
  for (int i = 1; i < 13; ++i) {
    Serial.print(i);
    Serial.print(" - ");    
    Serial.println(analogRead(i));
  }
  delay(500);

}

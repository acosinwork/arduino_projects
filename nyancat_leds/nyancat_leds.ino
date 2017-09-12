int ar[] = {
  0, A2, A0, A1
};

bool b = false;
void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 4; ++i)
  {
    pinMode(ar[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (b = !b) {
    digitalWrite(A0, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(0, LOW);
    digitalWrite(A1, LOW);

  } else {
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(0, HIGH);
    digitalWrite(A1, HIGH);
  }

  delay(800);

}

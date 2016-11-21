

uint8_t row[] = {
  //6, 7, 8, 9, 10, 11, 12, 13
  10, 7, 12, 6, A3, 13, 3, A0
};


uint8_t col[] = {
  2, 8, 9, A1, 11, A2, 4, 5

};


void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < 8; i++) {
    pinMode(row[i], OUTPUT);
    pinMode(col[i], OUTPUT);
    digitalWrite(col[i], HIGH);
  }
  Serial.begin(9600);
}

void loop() {


  for (int i = 0; i < 8; i++) {
    digitalWrite(row[i], HIGH);
    digitalWrite(col[i], LOW);
    delay(100);
    digitalWrite(row[i], LOW);
    digitalWrite(col[i], HIGH);
  }
}

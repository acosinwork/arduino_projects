
#define SPEED_1 = 5;
#define SPEED_2 = 6;

#define DIR_1 = 4;
#define DIR_2 = 7;


void setup() {                
  for (int i=4; i<8; ++i) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
    for (int i=4; i<8; ++i) {
    digitalWrite(i, !digitalRead(i));
  }

uint8_t a = analogRead(A5)>>2;
Serial.println(a);
  analogWrite(5,a);
  analogWrite(6,a);
 // delay(500);
  
 delay(1000);
  Serial.print(analogRead(A0));
  Serial.print("\t");
  Serial.print(analogRead(A1));
  Serial.println();
  

  digitalWrite(5, 0);
  digitalWrite(6, 0);
  delay(2000);
}



#define SPEED_1 = 5;
#define SPEED_2 = 6;

#define DIR_1 = 4;
#define DIR_2 = 7;


void setup() {                
  for (int i=4; i<8; ++i) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  
    for (int i=4; i<8; ++i) {
    digitalWrite(i, !digitalRead(i));
  }
  
  delay(1000);
  

}


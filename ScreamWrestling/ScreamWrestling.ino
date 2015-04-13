#include <Servo.h>

#define MIC_LEFT A0
#define MIC_RIGHT A5
#define SERVO_PIN 8
#define LED_PIN 7
#define AVERAGE 256

int lastLoudnessLeft=0;
int lastLoudnessRight=0;

unsigned long time=0;

Servo cursor;

void setup() {
  
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  cursor.attach(SERVO_PIN);
  cursor.write(90);  
  
//  while(true)
//  {
//    ;
//  }
  delay(2000);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  unsigned long loudnessLeft = 0;
  unsigned long loudnessRight = 0;
  
  for (int i=0; i < AVERAGE; ++i) {
    loudnessLeft += analogRead(MIC_LEFT);
    delay(1);
    loudnessRight += analogRead(MIC_RIGHT);
   // delay(1);    
  }  
  
  loudnessLeft /= AVERAGE;
  loudnessRight /= AVERAGE;
  
  lastLoudnessRight = loudnessRight;
  lastLoudnessLeft = loudnessLeft;
  
  int diff = -lastLoudnessLeft + lastLoudnessRight;
    
  int servoRotate = -map(loudnessLeft, 0, 1023, 0, 90) + map(loudnessRight, 0, 1023, 0, 90);
  
  Serial.print(lastLoudnessLeft);
  Serial.print('-');
  Serial.println(lastLoudnessRight);
  
  cursor.write(90 + servoRotate); 

}

#include <Wire.h>
#include <Servo.h>

Servo xaxa;

#define WORK
#define COM
#define BASESPEED 100 //50
#define K1 0.015//0.015
#define KUB 0.00000017//0.00000017
#define POVOROT 600  //600
#define GPUX_TWI_ADDR   0x20


int boost = 0;
int diff = 0;
int angles = 0;
int time = 0;

void setup(){
    
    xaxa.attach(6);
    xaxa.write(180);
    
    pinMode(12,OUTPUT);
    pinMode(4, OUTPUT);
    DDRE |= _BV(2);

    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    
      Wire.begin();

    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(0x03); // i/o config mode
    Wire.write(0x0F); // high 4 bits are leds, i.e. outputs
    Wire.endTransmission();
 #ifdef COM
    Serial.begin(9600);
 #endif
 
 #ifdef WORK
    diff = analogRead(2)-analogRead(3); 
 #endif
}
void loop(){
  //drive(-255,0);
  //2 - left analog
 // 3 - right analog 
 // 11 - digital sensor
 // белый - 1
 // чёрный - 0
 //пустота - 0
   int e = analogRead(2)-analogRead(3);//+(1-digitalRead(11))*POVOROT;//-diff;
   float u = K1*e + KUB*pow(e,3);
   int leftSpeed = BASESPEED+int(u);
   int rightSpeed = BASESPEED-int(u);
 // #ifdef COM
    Serial.print("angles=");
    Serial.println(angles);
    Serial.print("e=");
    Serial.println(e);
    Serial.print("u=");
    Serial.println(u);
    Serial.print("leftSpeed");
    Serial.println(leftSpeed);
    Serial.print("rightSpeed=");
    Serial.println(rightSpeed);
    Serial.print("2=");
    Serial.println(analogRead(2));
    Serial.print("3=");
    Serial.println(analogRead(3));
    Serial.print("11=");
    Serial.println(digitalRead(11));
    Serial.print("Servo = ");
    Serial.println(xaxa.read());
    delay(1500);
 // #endif
  
  #ifdef WORK
  if(angles==1&&(millis()-time)>1000)
    xaxa.write(180);
  //if black...
   if(digitalRead(11)==0){
     angles++;
     xaxa.write(60);
     if(angles==1)       
       time = millis();
   }

   //rightSpeed*=K3;
   leftSpeed = constrain(leftSpeed,-255,255);
   rightSpeed = constrain(rightSpeed,-255,255);
   drive(leftSpeed,rightSpeed);
  #endif
}

void drive(int speedLeft,int speedRight){
  if(speedLeft>0){
    digitalWrite(4,1);
  }else{
    digitalWrite(4,0);
  }

    if (speedRight>0)
        PORTE &= ~_BV(2);
    else
        PORTE |= _BV(2);


    analogWrite(9,abs(speedLeft));
    analogWrite(10,abs(speedRight));
} 
void ledWrite(byte idx, bool state)
{
    Wire.beginTransmission(GPUX_TWI_ADDR);
    Wire.write(0x01); // output write mode
    Wire.write(~(state << (7 - idx)));
    Wire.endTransmission();
}

int pause=300;
unsigned long time;
int relPin=A5;

void setup() {
  // put your setup code here, to run once:
  pinMode(relPin, OUTPUT);
  
  time=millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis()-time)>pause)
 {
   digitalWrite(relPin, !digitalRead(relPin));
   time=millis();
 } 
 
 pause = map(analogRead(A0), 0, 1023, 300, 3000);

}


ISR(TIMER4_COMPD_vect)
{
 digitalWrite(13, HIGH);
}
ISR(TIMER4_OVF_vect)
{
 digitalWrite(13, LOW);
}




void setup() {
  // put your setup code here, to run once:
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);

pinMode(6, OUTPUT);
pinMode(13, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  for (int i=0; i<256; ++i)
  {
  analogWrite(6,i);
  delay(1);
  }
  digitalWrite(13,LOW);
  
  delay(100);
  

}

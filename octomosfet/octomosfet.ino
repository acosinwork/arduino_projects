uint8_t a = 1;
uint8_t b=1;
void setup() {
  // put your setup code here, to run once:

  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(4, LOW);
shiftOut(7,5, MSBFIRST, a);
a<<=1;
if (a==0) a=b+=1;
  digitalWrite(4, HIGH);
  delay(100);
}

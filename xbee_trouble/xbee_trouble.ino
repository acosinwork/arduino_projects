
void setup() {
Serial1.begin(9600);
}
void loop() { 
  static int i=0;
Serial1.println(i); 
Serial1.flush();
delay(1000);
++i;
}

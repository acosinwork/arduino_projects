
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
/*  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/
  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600);

//  pinMode(4, OUTPUT);
//  digitalWrite(4, LOW);
}

void loop() { 
  // run over and over
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  while (Serial.available()) {
    Serial1.write(Serial.read());
  }

}


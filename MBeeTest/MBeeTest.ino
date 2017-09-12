
byte commandHi[] = {
  0x7E
  , 0x00
  , 0x08
  , 0x17
  , 0x01
  , 0x00
  , 0x02
  , 0x04
  , 0x52
  , 0x30
  , 0x05
  , 0x5A
};


byte commandLow[] = {
  0x7E
  , 0x00
  , 0x08
  , 0x17
  , 0x01
  , 0x00
  , 0x02
  , 0x04
  , 0x52
  , 0x30
  , 0x04
  , 0x5B
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial.available())
  {
    ;;
  }
  Serial1.begin(9600);

  Serial1.print("+");
  delay(10);
  Serial1.print("+");
  delay(10);
  Serial1.print("+");

  delay(200);
  Serial1.print("AT AP 1\r");
  delay(100);
  while (Serial1.available()) {
    Serial.println(Serial1.read());
  }
//  Serial1.print("AT AC\r");
  delay(100);

  Serial1.print("AT CN\r");


  while (Serial1.available()) {

    Serial.println(Serial1.read());
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 12; ++i)
    Serial1.write(commandHi[i]);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  Serial.println();
  delay(1000);


  for (int i = 0; i < 12; ++i)
    Serial1.write(commandLow[i]);

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  Serial.println();
  delay(1000);
}

 
void setup() 
{
  Serial.begin(9600);

}
 
void loop() 
{
      Serial.println("\xc0\xc1\xc2\xc3\xc4\xc5");
    Serial.println("\xc6\xc7\xc8\xc9\xca\xcb");
    delay(190);
       
      Serial.println("\xcc\xce\xcf\xd0\xd1\xd2");

      Serial.println("\xd3\xd4\xd5\xd6\xd7\xd8");
      delay(190);
}

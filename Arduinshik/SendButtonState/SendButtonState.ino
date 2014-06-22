//Значит у нас две ардуины. Одна- приёмник со светодиодом,
// а другая - передатчик с кнопкой.
// допустим это у нас:

//==== П Е Р Е Д А Т Ч И К ==========

#define BUTTON_PIN 2 // кнопка

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(9600);
}

void loop()
{
  bool isButtonDown = digitalRead(BUTTON_PIN);

  Serial.print(isButtonDown);
  delay(100);

} 


//Значит у нас две ардуины. Одна- приёмник со светодиодом,
// а другая - передатчик с кнопкой.
// допустим это у нас:

//==== П Р И Ё М Н И К ==========

#define LAMP_PIN 5 // светодиод

bool lampState = false;
bool wasButtonDown = false;

void setup()
{
  pinMode(LAMP_PIN, OUTPUT);
}

void loop()
{


  if (isButtonDown && !wasButtonDown) {
    lampState = !lampState;
    delay(10);
  }

  wasButtonDown = isButtonDown;
  digitalWrite(LAMP_PIN, lampState);
} 


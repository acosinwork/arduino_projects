//Значит у нас две ардуины. Одна- приёмник со светодиодом,
// а другая - передатчик с кнопкой.
// допустим это у нас:

//==== П Е Р Е Д А Т Ч И К ==========

//#define ROLE 0  // роль - приёмник
#define ROLE 1  // роль - передатчик

#define BUTTON_PIN 2 // кнопка
#define LAMP_PIN 5 // светодиод


bool lampState = false;
bool wasButtonDown = false;

void setup()
{
  prepareCommunication();

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LAMP_PIN, OUTPUT);
}

void loop()
{

  if (ROLE) //Если ардуина передатчик
  {
    transmitterRole();
  }
  else      // Если ардуина приёмник
  {
    recieverRole();
  }

} 

void  prepareCommunication()
{
  if (ROLE) //Если ардуина передатчик
  {
    Serial.begin(9600); 
  }
  else      // Если ардуина приёмник
  {
    Serial.begin(9600); //в случае Serial() - тот же самый код
  }
}

void transmitterRole()
{
  bool isButtonDown = digitalRead(BUTTON_PIN);

  sendButtonState(isButtonDown);
  delay(100);
}

void recieverRole()
{
  if (isButtonDown && !wasButtonDown) {
    lampState = !lampState;
    delay(10);
  }

  wasButtonDown = isButtonDown;
  digitalWrite(LAMP_PIN, lampState);

}

void sendButtonState(bool buttonState)
{
  Serial.print(buttonState);
}





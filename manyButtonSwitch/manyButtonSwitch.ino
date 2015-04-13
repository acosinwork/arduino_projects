#define CHECK_PERIOD  10
#define SWITCHES      4

class ButtonSwitch
{
  public:
    ButtonSwitch(int buttonPin, int ledPin)
    {
      _buttonPin = buttonPin;
      pinMode(_ledPin = ledPin, OUTPUT);
      _buttonWasUp = true;
    }

    void check()
    {
      if (millis() - _lastCheckTime > CHECK_PERIOD)
      {
        boolean buttonIsUp = digitalRead(_buttonPin);
        if (_buttonWasUp && !buttonIsUp)
          digitalWrite(_ledPin, !digitalRead(_ledPin));
        _buttonWasUp = buttonIsUp;
        _lastCheckTime = millis();
      }
    }

  private:
    int _buttonPin;
    int _ledPin;
    boolean _buttonWasUp;
    unsigned long _lastCheckTime;
};

ButtonSwitch buttons[SWITCHES] = {
  ButtonSwitch(Y1, Z1), ButtonSwitch(Y2, Z2),
  ButtonSwitch(Y3, Z3), ButtonSwitch(Y4, Z4)
};

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < SWITCHES; ++i)
    buttons[i].check();

}

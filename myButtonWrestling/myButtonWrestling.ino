#define CHECK_PERIOD  10
class Player
{
  public:
    Player(int buttonPin)
    {
      _buttonPin = buttonPin;
      _clicks = 0;
      _buttonWasUp = true;
    }

    void checkButton()
    {
      if (millis() - _lastCheckTime > CHECK_PERIOD)
      {
        boolean buttonIsUp = digitalRead(_buttonPin);
        if (_buttonWasUp && !buttonIsUp)
          _clicks += 1;
        buttonWasUp = buttonIsUp;
        _lastCheckTime = millis();
      }
    }
    
    int getClicks()
    {
      return _clicks;
    }

  private:
    int _buttonPin;
    int _clicks;
    boolean _buttonWasUp;
    unsigned long _lastCheckTime;

}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

#define TIMER_MIN 30

bool flow = false;
bool timeout = false;
byte bright = 0;
unsigned long delayButton = 0;

unsigned long pomodoro = 0;
unsigned long POMO_TIME = 1800000;

void setup() {
  // put your setup code here, to run once:
  pinMode(A4, OUTPUT);
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  if (!digitalRead(A3)) {
    if (millis() - delayButton > 50) {
      if (flow = !flow) {
        pomodoro = millis();
      } else {
        digitalWrite(A4, LOW);
        digitalWrite(13, LOW);
      }
    }
    delayButton = millis();
  }

  if ((flow) && (millis() - pomodoro > POMO_TIME)) {
    digitalWrite(A4, HIGH);
  }

  if (flow) {
    delay(1);
    analogWrite(13, bright++);
  }
}

#define INTERVAL_MICROS 20000
const byte interruptPin = 7;
const byte pwmPin = 10;
volatile uint16_t intCount = 0;

float k = (float)12 / 1000000;

unsigned long currentTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(10, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), inc, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((micros() - currentTime) >= INTERVAL_MICROS) {
    unsigned long utime = micros() - currentTime;
    currentTime = micros();
    float freq = (float)intCount/((float)utime*k);
    intCount = 0;
    analogWrite(pwmPin, map(analogRead(A0),0,1024,0,255));
    Serial.println(freq);
  }
}

void inc() {
  ++intCount;
}


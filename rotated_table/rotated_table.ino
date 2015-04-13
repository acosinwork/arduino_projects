#define STEP 5
#define DIR  6
#define EN   7
#define SHOT 8

#define STEPS_REVOLUTION 400

#define SHOTS 40

#define STEPS_ON_SHOT STEPS_REVOLUTION/SHOTS

#define DELAY_TIME 8



void setup() {
  // put your setup code here, to run once:
  for (int i = STEP; i <= EN; ++i) {
    pinMode(i, OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(EN, HIGH);
  
  for (int i = 0; i < SHOTS; ++i) {
    delay(500);
    digitalWrite(SHOT, HIGH);
    delay(200);
    digitalWrite(SHOT, LOW);
    delay(500);
    
    step(STEPS_ON_SHOT);

  }
  digitalWrite(EN, LOW);

  while (true) {

  }


}

void step(int count)
{
  for (int i=0; i< count; ++i) {
    digitalWrite(STEP, HIGH);
    delay(DELAY_TIME);
    
    digitalWrite(STEP, LOW);
    delay(DELAY_TIME);
  }
}

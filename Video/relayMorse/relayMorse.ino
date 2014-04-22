
#define DOTS = 1
#define DASHES = 3
#define SPACE = 5
#define MORSE_LENGTH_MASK = 0b00000111

int dotLengthMillis = 100;

byte morseCharCode[] =
{
  // _ - 1; . - 0
  //_____111; _ - code, 1 - symbolCount
  0b01000010, // A ._
  0b10000100, // B _...
  0b10100100, // C _._.
  0b10000011, // D _..
  0b00000001, // E .
  0b00100100, // F .._.
  0b11000011, // G _ _.
  0b00000100, // H ....
  0b00000010, // I ..
  0b01110100, // J ._ _ _
  0b10100011, // K _._
  0b01000010, // L ._..
  0b11000010, // M _ _
  0b10000010, // N _.
  0b11100011, // O _ _ _
  0b01100100, // P ._ _.
  0b11010100, // Q _ _ . _
  0b01000011, // R ._.
  0b00000011, // S ...
  0b10000001, // T _
  0b00100011, // U .._
  0b00010100, // V ..._
  0b01100011, // W ._ _
  0b10010100, // X _.._
  0b10110100, // Y _._ _
  0b11000100 // Z _ _..
};

byte morseNumCode[] =
{
  0b11111101, // 0 _ _ _ _ _
  0b01111101, // 1 ._ _ _ _
  0b00111101, // 2 .._ _ _
  0b00011101, // 3 ..._ _
  0b00001101, // 4 ...._
  0b00000101, // 5 .....
  0b10000101, // 6 _....
  0b11000101, // 7 _ _...
  0b11100101, // 8 _ _ _..
  0b11110101  // 9 _ _ _ _.
};

byte relayPin = 13;
unsigned long time;
byte incomingByte;      // a variable to read incoming serial data into

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  time = millis();

}

void loop() {
  // put your main code here, to run repeatedly:

  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    //    Serial.write(incomingByte);
    
    if ((incomingByte>96) && (incomingByte<123) // small char
    {
      incomingByte = incomingByte - 32; // convert to BIG CHAR
    }
    
    if ((incomingByte>64) && (incomingByte<91) // BIG CHAR
    {
      incomingByte = incomingByte - 65; // to morseCharCode adress
    }
    

  }
  

}

void morsePrint(byte printByte);
{
  byte morseCharLength = printByte & MORSE_LENGTH_MASK;
  
  for (byte i=0; i<morseCharLength; i++)
  {
    if
  }
  
}


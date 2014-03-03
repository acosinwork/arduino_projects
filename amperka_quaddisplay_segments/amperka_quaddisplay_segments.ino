const byte PIN_CTL = A0;

const byte numbers[] = {
  0b00000011,  //0
  0b10011111,  //1
  0b00100101,  //2
  0b00001101,  //3
  0b10011001,  //4
  0b01001001,  //5
  0b01000001,  //6
  0b00011111,  //7
  0b00000001,  //8
  0b00001001   //9
};

void sendByte(byte data, const byte n = 8)
{
  for (byte i = n; i > 0; i--)
  {
    if (data & 1)
    {
      noInterrupts();
      digitalWrite(PIN_CTL, LOW);
      digitalWrite(PIN_CTL, HIGH);
      interrupts();
      delayMicroseconds(30);
    }
    else
    {
      digitalWrite(PIN_CTL, LOW);
      delayMicroseconds(15);
      digitalWrite(PIN_CTL, HIGH);
      delayMicroseconds(60);
    }
    data >>= 1;
  }
}

void latch()
{
  digitalWrite(PIN_CTL, LOW);
  delayMicroseconds(100);
  digitalWrite(PIN_CTL, HIGH);
  delayMicroseconds(300);
}

void clearDisplay()
{
  for (byte y = 0; y<4; ++y){
    sendByte(0b11111111);
  }
  latch();
}

void displayInt(int int_data)
{
  byte displayValue[] = {
    0b11111111,
    0b11111111, 
    0b11111111, 
    0b11111111  
  };
  byte capacity;
  
  int pure_data=int_data;

  if ((pure_data>999)&&(pure_data<10000)) 
  {

    capacity=int_data/1000;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[0]=numbers[capacity];
      int_data=int_data - (capacity*1000);
    }
  }
  if (pure_data>99) 
  {
    capacity=int_data/100;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[1]=numbers[capacity];
      int_data=int_data - (capacity*100);
    }
  }
  if (pure_data>9) 
  {
    capacity=int_data/10;  

    if ((capacity >= 0)&&(capacity <10))
    {
      displayValue[2]=numbers[capacity];
      int_data=int_data - (capacity*10);
    }
  }
  capacity=int_data;  

  if ((capacity >= 0)&&(capacity <10))
  {
    displayValue[3]=numbers[capacity];
  }


  for (capacity=0; capacity<4; ++capacity)
  {
    sendByte(displayValue[capacity]);
  }
  latch();
}

void setup()
{
  pinMode(PIN_CTL, OUTPUT);
  digitalWrite(PIN_CTL, HIGH);

  clearDisplay();
  delay(2000);
}

void loop()
{

  //  for (byte z = 0; z<7; ++z){  
  for (int i = 0; i<9999; ++i) {
    displayInt(i);

    delay(10);
  }

}







/*
 data_t x = 0xffffffffUL;
 for (byte i = 0; i < n_bits; i++)
 {
 data_t y = x;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff);
 //sendByte(0xff,2);
 latch();
 delay(500);
 x = x>>1;// | 0x80;
 }
 x = 0x80000000UL;
 for (byte i = 0; i < n_bits; i++)
 {
 //x <<= 1;
 x = x>>1 | 0x80000000UL;
 data_t y = x;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff); y >>= 8;
 sendByte(y & 0xff);
 //sendByte(0xff,2);
 latch();
 delay(500);
 }
/*
 sendByte(0xff);
 sendByte(0x00);
 sendByte(0xff);
 sendByte(0x00);
 latch();
 delay(500);
 sendByte(0x00);
 sendByte(0xff);
 sendByte(0x00);
 sendByte(0xff);
 latch();
 delay(500);
 */
//}







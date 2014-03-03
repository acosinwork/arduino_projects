/*
  I2C Touch Sensor Demo v1.0
  By:http://www.seeedstudio.com
  Connect the I2C touch Sensor to I2C Grove connector to use
  this demo use the software I2C to communicate.
  
*/
 
#include <mpr121.h>
#include <i2c.h>
//#include <Wire.h>
#define MPR121_R 0xB5	// ADD pin is grounded
#define MPR121_W 0xB4	// So address is 0x5A
#define MPR121addr    0x5a
int StateOfTouch1 =0;
int StateOfTouch2 =0;
int StateOfTouch3 =0;
int StateOfTouch4 =0;
int StateOfTouch5 =0;
int StateOfTouch6 =0;
int StateOfTouch7 =0;
int StateOfTouch8 =0;
int StateOfTouch9 =0;
int StateOfTouch10 =0;
int StateOfTouch11 =0;
int StateOfTouch12 =0;
int irqpin = 2;  // D2
uint16_t touchstatus;
volatile int GetTouchSignal = 0;
unsigned char reg = 0;
//int TouchedSensor = 0;
unsigned char status1 =0;
unsigned char status2 =0;
long previousMillis = 0;
long interval = 100;
/*void Write_MPR121(unsigned char address,unsigned char data)
{
  Wire.beginTransmission(MPR121addr);
  Wire.send(address);
  Wire.send(data);
  Wire.endTransmission();
}*/
void setup()
{
//  pinMode(irqpin, INPUT);
//  digitalWrite(irqpin, HIGH);
//  Wire.begin();
  Serial.begin(9600);
  DDRC |= 0b00010011;
  PORTC = 0b00110000;  // Pull-ups on I2C Bus
  i2cInit();
  Serial.println("setting device up..");
  delay(100);
  mpr121QuickConfig();
 // mpr121Write(0x5e,0x00);
 // mpr121Write(0x5d,0x23);
//  Write_MPR121(0x5e, 0x41);
}
void loop()
{
  //Serial.println(" in loop..");
   unsigned char MPR_Query=0;
//   checkInterrupt();
  unsigned long currentMillis = millis();
  // if(GetTouchSignal == 1)
 if(currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
 //   GetTouchSignal = 0 ;
  //  Serial.println("get a ISR in loop..");
   // TouchedSensor = Read_MPR121();
    Read_MPR121();
  }
  //getPhoneNumber();
   if (Serial.available()>0)
  {
    Serial.println("Get a command form PC..");
    reg = Serial.read();
    Serial.print("Quiry the Reg of ");
    Serial.println(reg,BIN);
//  Serial.print("\nDialing... ");
  //for (int i=0; i<PHONE_DIGITS; i++)
  //Serial.print(phoneNumber[i]);
  MPR_Query = mpr121Read(reg);
  Serial.print("the reg is = 0x");
  Serial.println(MPR_Query,HEX);

  //while(1);
//  delay(1000);
   }
   delayMicroseconds(1000);
}
void Read_MPR121()
{
//  Serial.println("INTR deteced in main.,begin to read reg 00 and 01");
//  Wire.beginTransmission( MPR121addr);
//  Wire.send(MPR121_touchstatus1);
//  Wire.send(0x8f);
//  Wire.requestFrom(MPR121addr,2);
//  while(Wire.available() == 2)
 // {
 //   Serial.println("receive the two reg 00 and 01");
 //   status1 = Wire.receive();
 //   status2 = Wire.receive();
 // }
 // Wire.endTransmission();
 status1=mpr121Read(0x00);
 status2=mpr121Read(0x01);
 //Serial.print("status 1 is ");
// Serial.println(status1);
// Serial.print("status 2 is ");
 //Serial.println(status2);
 CheckStatus();  
}
void  CheckStatus()
{
   if ((status1&0x01)==0x01)
  {
      StateOfTouch1 =1; 
 }
  else
 {
    if (StateOfTouch1 ==1)
    {
     Serial.println("channel 0 has pressed.."); 
     StateOfTouch1 =0;
    }
  }
    if ((status1&0x02)==0x02)
  {
      StateOfTouch2 =1; 
  }
  else
  {
    if (StateOfTouch2 ==1)
    {
     Serial.println("channel 1 has pressed.."); 
     StateOfTouch2 =0;
    }
  }
      if ((status1&0x04)==0x04)
  {
      StateOfTouch3 =1; 
  }
    else
    {
    if (StateOfTouch3 ==1)
      {
       Serial.println("channel 2 has pressed.."); 
       StateOfTouch3 =0;
      }
    }
      if ((status1&0x08)==0x08)
  {
      StateOfTouch4 =1; 
  }
  else
  {
    if (StateOfTouch4 ==1)
    {
     Serial.println("channel 3 has pressed.."); 
     StateOfTouch4 =0;
    }
  }
      if ((status1&0x10)==0x10)
  {
      StateOfTouch5 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch5 ==1)
    {
     Serial.println("channel 4 has pressed.."); 
     StateOfTouch5 =0;
    }
  }
      if ((status1&0x20)==0x20)
  {
      StateOfTouch6 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch6 ==1)
    {
     Serial.println("channel 5 has pressed.."); 
     StateOfTouch6 =0;
    }
  }
    if ((status1&0x40)==0x40)
  {
      StateOfTouch7 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch7 ==1)
    {
     Serial.println("channel 6 has pressed.."); 
     StateOfTouch7 =0;
    }
  }
      if ((status1&0x80)==0x80)
  {
      StateOfTouch8 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch8 ==1)
    {
     Serial.println("channel 7 has pressed.."); 
     StateOfTouch8 =0;
    }
  }
 //  else if ((status1&0x04)==0x04)
 // {
 //  Serial.println("channel 3 has pressed..");
 // }
 //  else if ((status1&0x08)==0x08)
 // {
  //  Serial.println("channel 4 has pressed..");
 // }
 //  else if ((status1&0x10)==0x10)
 // {
  //  Serial.println("channel 5 has pressed..");
 /// }
 //  else if ((status1&0x20)==0x20)
 // {
 //   Serial.println("channel 6 has pressed..");
 // }
 //  else if ((status1&0x40)==0x40)
 // {
 //   Serial.println("channel 7 has pressed..");
 // }
 //  else if ((status1&0x80)==0x80)
 // {
 //   Serial.println("channel 8 has pressed..");
 // }
    if ((status2&0x01)==0x01)
  {
      StateOfTouch9 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch9 ==1)
    {
     Serial.println("channel 8 has pressed.."); 
     StateOfTouch9 =0;
    }
  }
      if ((status2&0x02)==0x02)
  {
      StateOfTouch10 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch10 ==1)
    {
     Serial.println("channel 9 has pressed.."); 
     StateOfTouch10 =0;
    }
  }
      if ((status2&0x04)==0x04)
  {
      StateOfTouch11 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch11 ==1)
    {
     Serial.println("channel 10 has pressed.."); 
     StateOfTouch11 =0;
    }
  }
      if ((status2&0x08)==0x08)
  {
      StateOfTouch12 =1; 
  //  Serial.println("channel 2 has pressed..");
  }
  else
  {
    if (StateOfTouch12 ==1)
    {
     Serial.println("channel 11 has pressed.."); 
     StateOfTouch12 =0;
    }
  }
 //  else if ((status2&0x02)==0x02)
 // {
 //   Serial.println("channel 10 has pressed..");
 // }
 //  else if ((status2&0x04)==0x04)
 // {
 //   Serial.println("channel 11 has pressed..");
 // }
 //  else if ((status2&0x08)==0x08)
 // {
 //   Serial.println("channel 12 has pressed..");
 // }  
//  end if ;
}
byte mpr121Read(uint8_t address)
{
  byte data;

  i2cSendStart();
  i2cWaitForComplete();

  i2cSendByte(MPR121_W);	// write 0xB4
  i2cWaitForComplete();

  i2cSendByte(address);	// write register address
  i2cWaitForComplete();

  i2cSendStart();

  i2cSendByte(MPR121_R);	// write 0xB5
  i2cWaitForComplete();
  i2cReceiveByte(TRUE);
  i2cWaitForComplete();

  data = i2cGetReceivedByte();	// Get MSB result
  i2cWaitForComplete();
  i2cSendStop();

  cbi(TWCR, TWEN);	// Disable TWI
  sbi(TWCR, TWEN);	// Enable TWI

  return data;
}

void mpr121Write(unsigned char address, unsigned char data)
//void mpr121Write(uint8_t address, uint8_t data)
{
  i2cSendStart();
  i2cWaitForComplete();

  i2cSendByte(MPR121_W);// write 0xB4
  i2cWaitForComplete();

  i2cSendByte(address);	// write register address
  i2cWaitForComplete();

  i2cSendByte(data);
  i2cWaitForComplete();

  i2cSendStop();
  //cbi(TWCR, TWEN);	// Disable TWI
 // sbi(TWCR, TWEN);	// Enable TWI
}

void mpr121QuickConfig(void)
{
  Serial.println("config MPR121..");
  // Section A
  // This group controls filtering when data is > baseline.
  mpr121Write(MHD_R, 0x01);
  mpr121Write(NHD_R, 0x01);
  mpr121Write(NCL_R, 0x00);
  mpr121Write(FDL_R, 0x00);

  // Section B
  // This group controls filtering when data is < baseline.
  mpr121Write(MHD_F, 0x01);
  mpr121Write(NHD_F, 0x01);
  mpr121Write(NCL_F, 0xFF);
  mpr121Write(FDL_F, 0x02);

  // Section C
  // This group sets touch and release thresholds for each electrode
  mpr121Write(ELE0_T, TOU_THRESH);
  mpr121Write(ELE0_R, REL_THRESH);
  mpr121Write(ELE1_T, TOU_THRESH);
  mpr121Write(ELE1_R, REL_THRESH);
  //mpr121Write(ELE1_T, 0x06);
  //mpr121Write(ELE1_R, 0x03);
  mpr121Write(ELE2_T, TOU_THRESH);
  mpr121Write(ELE2_R, REL_THRESH);
  mpr121Write(ELE3_T, TOU_THRESH);
  mpr121Write(ELE3_R, REL_THRESH);
  mpr121Write(ELE4_T, TOU_THRESH);
  mpr121Write(ELE4_R, REL_THRESH);
  mpr121Write(ELE5_T, TOU_THRESH);
  mpr121Write(ELE5_R, REL_THRESH);
  mpr121Write(ELE6_T, TOU_THRESH);
  mpr121Write(ELE6_R, REL_THRESH);
  mpr121Write(ELE7_T, TOU_THRESH);
  mpr121Write(ELE7_R, REL_THRESH);
  mpr121Write(ELE8_T, TOU_THRESH);
  mpr121Write(ELE8_R, REL_THRESH);
 // mpr121Write(ELE8_T, 0x06);
 // mpr121Write(ELE8_R, 0x03);
  mpr121Write(ELE9_T, TOU_THRESH);
  mpr121Write(ELE9_R, REL_THRESH);
  mpr121Write(ELE10_T, TOU_THRESH);
  mpr121Write(ELE10_R, REL_THRESH);
  mpr121Write(ELE11_T, TOU_THRESH);
  mpr121Write(ELE11_R, REL_THRESH);
  //mpr121Write(0x7D,0xc9);
  //mpr121Write(0x7E,0x82);
  //mpr121Write(0x7F,0xb5);
 // mpr121Write(0x7B,0x0b);
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  //mpr121Write(FIL_CFG, 0x24);
 
  mpr121Write(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   mpr121Write(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
  mpr121Write(ATO_CFGL, 0x82);  // Target = 0.9*USL = 0xB5 @3.3V
  mpr121Write(ATO_CFGT,0xb5);
  mpr121Write(ATO_CFG0, 0x1B);
  // Section E
  // Electrode Configuration
  // Enable 6 Electrodes and set to run mode
  // Set ELE_CFG to 0x00 to return to standby mode
  //mpr121Write(ELE_CFG, 0x0C);	// Enables all 12 Electrodes
  mpr121Write(ELE_CFG, 0x8c);
  //mpr121Write(ELE_CFG, 0x06);		// Enable first 6 electrodes

  // Section F
  // Enable Auto Config and auto Reconfig
  /*mpr121Write(ATO_CFG0, 0x0B);
   mpr121Write(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   mpr121Write(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
   mpr121Write(ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
   Serial.println("config MPR121 Done!");
}

void  checkInterrupt(void)
{
  //Serial.println("checking the interrupt..");
  int IRQ =0;
  IRQ = digitalRead(irqpin);
  if(IRQ == 0)
  {
    GetTouchSignal=1;
  }
 // return 0;
  else
  {
    GetTouchSignal=0;
  }
 //  Serial.print("gettouchsignal is ");
  // Serial.println(GetTouchSignal);
}



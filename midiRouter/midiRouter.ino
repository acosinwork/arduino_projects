
/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>

#undef _SS_MAX_RX_BUFF  // it was 64, @SoftwareSerial

#define _SS_MAX_RX_BUFF 32  // for the great justice

#define MIDI_SPEED 31250

#define MIDI_CH_SIZE 4

#define MIDI_INP 0
#define MIDI_OUTP 1

byte midiPinIO[MIDI_CH_SIZE][2] = 
{
  {
    2, 3  }
  , {
    4, 5  }
  , {
    6, 7  }
  , {
    8, 9  }
};

SoftwareSerial* routeList[MIDI_CH_SIZE][MIDI_CH_SIZE];

SoftwareSerial* pMidiCh[MIDI_CH_SIZE];


/**
 * TODO: очередь маршрутов
 **/

void setup()  
{

  // Open serial communications and wait for port to open:
  Serial.begin(MIDI_SPEED);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  midiChPrepare();

  SPItoMIDIInit();


}

void loop() // run over and over
{
  virtualMidiRoute();

  hardwareMidiRoute();

}

void midiChPrepare()
{
  for (int i=0; i<MIDI_CH_SIZE; ++i)
  {
    pMidiCh[i]= new SoftwareSerial(midiPinIO[i][MIDI_INP], midiPinIO[i][MIDI_OUTP]);
    (*pMidiCh[i]).begin(MIDI_SPEED);

    for (int y=0; y<MIDI_CH_SIZE; ++y)
    {
      routeList[i][y] = 0;  
    }
  }
}

void addRoute( int midiChNum, SoftwareSerial* routedCh)
{
  for (byte i = 0; i<MIDI_CH_SIZE; ++i)
  {

    if (routeList[midiChNum][i] == routedCh)  //Если канал уже тут есть, выходим
      break; 

    if (!routeList[midiChNum][i])
    {
      routeList[midiChNum][i]=routedCh;   //А если нет - добавляем и выходим
      break;
    }
  }
}

void deleteRoute( int midiChNum, SoftwareSerial* routedCh)
{
  for (byte i = 0; i<MIDI_CH_SIZE; ++i)
  {

    if (routeList[midiChNum][i] == routedCh)
    {
      routeList[midiChNum][i] = 0; // удаляем указатель на миди канал по всему листу
    }
  }
}

void virtualMidiRoute()
{
  for (int i=0; i<MIDI_CH_SIZE; ++i)
  {
    while ((*pMidiCh[i]).available()>0)
    {
      byte message = (*pMidiCh[i]).read();

      for (int y=0; y<MIDI_CH_SIZE; ++y)
      {
        if (routeList[i][y])
        {
          (*routeList[i][y]).write(message);
        }
      }

      Serial.write(message); // По юсб данные передаются всегда
    }
  }

}

void hardwareMidiRoute()
{
  while (Serial.available()>0)
  {
    byte message = Serial.read();
    for (int i=0; i<MIDI_CH_SIZE; ++i)
    {
      (*pMidiCh[i]).write(message); //все данные с юсб идут на все выходы
    }
  }
}















/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

boolean isNeedTemperature=false;

#include <Ultrasonic.h>

#define ULTRAGND 5
#define ULTRAVCC 2
#define TRIG_PIN 3
#define ECHO_PIN 4

Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
float cmdistance=0;
unsigned int iter=0;

unsigned long time;



void setup() {
  
  sensors.begin();

  pinMode(ULTRAGND, OUTPUT);
  digitalWrite(ULTRAGND, LOW);

  pinMode(ULTRAVCC, OUTPUT);
  digitalWrite(ULTRAVCC, HIGH);
  
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);

  Serial.begin(9600);

  /*
   * Set the SPI Driver.
   */

  Mirf.spi = &MirfHardwareSpi;

  /*
   * Setup pins / SPI.
   */

  Mirf.init();

  /*
   * Configure reciving address.
   */

  Mirf.setRADDR((byte *)"serv1");

  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */

  Mirf.payload = sizeof(float);

  /*
   * Write channel and payload config then power up reciver.
   */

  Mirf.config();

  Serial.println("Listening...");
  
  time = millis();
}

void loop() {

  if ((millis()-time)>1000) 
  {
  sensors.requestTemperatures();
  time=millis();
  }
 
  float mmdistance;
  
  long microsec = ultrasonic.timing();
  cmdistance += ultrasonic.CalcDistance(microsec, Ultrasonic::CM);
  ++iter;
  
  delay(50);
  
//   Serial.println(cmdistance);
  /*
   * A buffer to store the data.
   */
  float button;
  //byte data[Mirf.payload];

  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it
   * transitions from true to false.
   */

  if (!Mirf.isSending() && Mirf.dataReady()) {

      Serial.println("Got packet");

    /*
     * Get load the packet into the buffer.
     */

    Mirf.getData((byte *) &button);
    
    if (button == float(65)) 
    {
      digitalWrite(6, HIGH);
      isNeedTemperature=true;
      
      
    } else if (button == float(97))
    {
      digitalWrite(6, LOW);
       isNeedTemperature=false;
    }

if (isNeedTemperature) {

    mmdistance = 10*sensors.getTempCByIndex(0);  
}
else
{
    mmdistance =10 * (cmdistance/iter); //this result unit is centimeter
  
}
    
    cmdistance=0;
    iter=0;

  
    /*
     * Set the send address.
     */


    Mirf.setTADDR((byte *)"clie1");

    /*
     * Send the data back to the client.
     */


    Mirf.send((byte *)&mmdistance);

    /*
     * Wait untill sending has finished
     *
     * NB: isSending returns the chip to receving after returning true.
     */

    Serial.println("Reply sent.");
    Serial.println(mmdistance);
 
    
  }
}

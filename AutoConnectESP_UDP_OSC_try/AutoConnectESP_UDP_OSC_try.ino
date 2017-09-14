#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

//#include <UDP.h>         //https://github.com/tzapu/WiFiManager
///#include <EthernetUDP.h>
#include <Wire.h>
#include <GpioExpander.h>
#include <OSCMessage.h>

#include <OSCBundle.h>
#include <OSCTiming.h>


#include <Ticker.h>

#define PAULSSUGGESTION

Ticker ticker;
OSCBundle bndl;
volatile bool wasTick = false;

void tick()
{
  wasTick = true;
}


GpioExpander adio(42);

int lastVal = 0;
int i;

unsigned int localPort = 3003;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back
IPAddress outIp(192, 168, 98, 102);
WiFiUDP Udp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  // Udp.begin(localPort);

  Wire.begin(0, 2);
  ticker.attach_ms(10, tick);
  //Udp.setNoDelay(1);
  Udp.begin(8888);
  adio.pinModePort(0x0000, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(0);

  if (wasTick) {

    uint64_t timetag;

    bndl.add("/1/fader1").add((float)(adio.analogRead(0)/4095.0)); //(0.5 + sin(++i / PI) * 0.5)); // / 2047.0));
    bndl.add("/1/fader2").add((float)(adio.analogRead(1)/4095.0)); //(0.5 + sin((i + 2) / PI) * 0.5)); // / 2047.0));
    bndl.add("/1/fader3").add((float)(adio.analogRead(2)/4095.0)); //(//0.5 + sin((i + 4) / PI) * 0.5)); // / 2047.0));
    bndl.add("/1/fader4").add((float)(adio.analogRead(3)/4095.0)); //(//0.5 + sin((i + 6) / PI) * 0.5)); // / 2047.0));

    Udp.beginPacket(outIp, localPort);
    bndl.setTimetag(oscTime());
    bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    Udp.flush();
    bndl.empty(); // empty the bundle to free room for a new one

    /*
        //   if (lastVal != aread) {

        OSCMessage msg("/1/fader1");
        msg.add((float)(0.5 + sin(++i / PI) * 0.5)); // / 2047.0));


        Udp.beginPacket(outIp, localPort);
        msg.send(Udp); // send the bytes to the SLIP stream
        Udp.endPacket(); // mark the end of the OSC Packet
        msg.empty(); // free space occupied by message
        //   lastVal = aread;
        //    }

    */
    wasTick = false;
  }
  //  delay(15);

}



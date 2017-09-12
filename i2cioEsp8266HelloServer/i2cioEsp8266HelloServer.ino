#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <I2Cadio.h>

ADIO adio(42);
const char* ssid = "Amperka";
const char* password = "mega2560";

ESP8266WebServer server(80);

const int led = 2;

void handleRoot() {
  adio.digitalWrite(led, 0);
  server.send(200, "text/plain", "hello from esp8266!");
  int i = adio.analogRead(6);
  delay(0);
  adio.analogWrite(led, i);
//  Serial.println();
}

void handleNotFound(){
  adio.digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  adio.digitalWrite(led, 0);
}

void setup(void){
  Wire.begin(0,2);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  adio.pinMode(led, OUTPUT);
  delay(1);
  adio.digitalWrite(led, 0);

}

void loop(void){
  server.handleClient();
}

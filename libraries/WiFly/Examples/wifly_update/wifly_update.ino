#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID      "Your-SSID"
#define KEY       "passphrase"
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
SoftwareSerial uart(2, 3);
WiFly wifly(uart);


void setup() {
  Serial.begin(9600);
  Serial.println("------ WIFLY UPDATE FIRMWARE -------");
  
  uart.begin(9600);     // WiFly UART Baud Rate: 9600
  // wait for initilization of wifly
//  delay(3000);
  
  float ver;
  Serial.print("WiFly Version: ");
  ver = wifly.version();
  Serial.println(ver);
  if (ver >= 2.45) {
    Serial.println("The WiFly's firmware is update");
    
    if (wifly.commandMode()) {
      Serial.println("Enter command mode. Send \"exit\"(with \\r) to exit command mode");
    }
    return;
  }
  
  Serial.println("WiFly Version: ");
  Serial.println(wifly.version());
  wifly.reset();
  
  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Succeed to join " SSID);
      wifly.clear();
      break;
    } else {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 3 second and try again...");
      delay(3000);
    }
  }
  
  Serial.println("ftp server: rn.microchip.com");
  wifly.sendCommand("set dns name rn.microchip.com\r");
  wifly.sendCommand("set ftp address 0\r");
  Serial.println("firmware: wifly-EZX.img or wifly-EZX-AP.img");
  wifly.sendCommand("set ftp filename wifly-EZX-AP.img\r");
  Serial.println("Try to update");
  wifly.sendCommand("ftp update\r");
}

void loop() {
  int c;
  while (wifly.available()) {
    c = wifly.read();
    if (c > 0) {
      Serial.write((char)c);
    }
  }
  
  while (Serial.available()) {
    c = Serial.read();
    if (c >= 0) {
      wifly.write((char)c);
    }
  }
}

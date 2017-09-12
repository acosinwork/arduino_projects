// Example by Tom Igoe

import processing.serial.*;

Serial myPort;  // The serial port

void setup() {
  size(500,500);
   colorMode(RGB, 255);
  // List all the available serial ports:
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);//
 // myPort.write(65);
}

void draw() {
  while (myPort.available() > 0) {
    int lf = 0;
    // Expand array size to the number of bytes you expect:
    byte[] inBuffer = new byte[4];
    
    int[] inBuf = new int[4];
    myPort.readBytesUntil(lf, inBuffer);
    if (inBuffer != null) {
//      String myString = new String(inBuffer);
      for (int i =0; i<4; ++i){
        inBuf[i] = inBuffer[i] &0xff;
      }

    fill(color(inBuf[0], inBuf[1], inBuf[2]));
    rect(0,0,500,500);
   //   println(myString);
    }
  }
}

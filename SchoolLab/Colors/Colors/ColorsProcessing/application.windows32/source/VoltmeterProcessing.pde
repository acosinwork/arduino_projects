//Program by Jeremy Blum
//www.jeremyblum.com
//Uses pot data from arduino to draw stuff

import processing.serial.*;
Serial port;
float nextValue = 0;
int sizeRect = 500;

String s;

FloatList oscill;

void setup()
{
  size(sizeRect, sizeRect);
  port = new Serial(this, "COM61", 9600);
  port.bufferUntil('\n');  
  oscill = new FloatList();
  s = new String("no data");
  textSize(18);
}

void draw()
{
  background(0);
  stroke(255);
  
  text(s, 10, 30);
  
  for (int i=0; i<oscill.size()-2; ++i) {
    line(i, (sizeRect-(oscill.get(i)*sizeRect/5)), i+1, (sizeRect-(oscill.get(i+1)*sizeRect/5)));
  }
}
  
void serialEvent (Serial port)
{
  nextValue = float(port.readStringUntil('\n'));
  
  s= nextValue + " V";
 
  
  if (!(oscill.size()<sizeRect)) {
    oscill.remove(0);
  }

  oscill.append(nextValue);
   
}

void stop() {
  port.clear();
  port.stop();
} 


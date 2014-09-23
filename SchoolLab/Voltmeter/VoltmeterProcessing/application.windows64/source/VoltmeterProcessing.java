import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class VoltmeterProcessing extends PApplet {

//Program by Jeremy Blum
//www.jeremyblum.com
//Uses pot data from arduino to draw stuff


Serial port;
float nextValue = 0;
int sizeRect = 500;

String s;

FloatList oscill;

public void setup()
{
  size(sizeRect, sizeRect);
  port = new Serial(this, "COM61", 9600);
  port.bufferUntil('\n');  
  oscill = new FloatList();
  s = new String("no data");
  textSize(18);
}

public void draw()
{
  background(0);
  stroke(255);
  
  text(s, 10, 30);
  
  for (int i=0; i<oscill.size()-2; ++i) {
    line(i, (sizeRect-(oscill.get(i)*sizeRect/5)), i+1, (sizeRect-(oscill.get(i+1)*sizeRect/5)));
  }
}
  
public void serialEvent (Serial port)
{
  nextValue = PApplet.parseFloat(port.readStringUntil('\n'));
  
  s= nextValue + " V";
 
  
  if (!(oscill.size()<sizeRect)) {
    oscill.remove(0);
  }

  oscill.append(nextValue);
   
}

public void stop() {
  port.clear();
  port.stop();
} 

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "VoltmeterProcessing" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}

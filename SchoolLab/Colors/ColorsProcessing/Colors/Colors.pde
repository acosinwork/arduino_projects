
import processing.serial.*;
Serial port;
float nextValue = 0;
int sizeRect = 1000;
Integer r=0;
Integer g=0;
Integer b=0;


//int[] rgb;

void setup()
{
  size(sizeRect, sizeRect);
  port = new Serial(this, "COM4", 115200);
  port.bufferUntil('\n');  
//  rgb = new int[3];
  strokeWeight(10);
  ellipseMode(CENTER);
  textAlign(CENTER, CENTER);
  textSize(20);
  //rectMode(RADIUS);
}

void draw()
{
  background(r,g,b);
  
  stroke(255,0,0);
  fill(r, 0, 0);
  ellipse(width/4, height/2, height/4, height/4);
  fill(255);
  text(r.toString(), width/4, height/2);
  
  stroke(0,255,0);
  fill(0, g, 0);
  ellipse(width/2, height/2, height/4, height/4);
  fill(255);
  text(g.toString(), width/2, height/2);
  
  stroke(0,0,255);
  fill(0, 0, b);
  ellipse(3*width/4, height/2, height/4, height/4);
  fill(255);
  text(b.toString(), 3*width/4, height/2);
  
}

void serialEvent (Serial port)
{
  try {
    String data = port.readStringUntil('\n');

    int[] rgb = int(split(data, ' '));
    
    r=rgb[0];
    g=rgb[1];
    b=rgb[2];
    
/*
    for (int i=0; i< 3; ++i)
    {
      rgb[i] = Integer.parseInt(list[i]);
    }
    */
  } 
  catch (Exception e) {
    println("Connection...");
  }
}

void stop() {
  port.clear();
  port.stop();
} 


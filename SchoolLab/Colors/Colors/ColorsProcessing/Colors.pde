
import processing.serial.*;
Serial port;
float nextValue = 0;
int sizeRect = 500;
int r=0;
int g=0;
int b=0;


//int[] rgb;

void setup()
{
  size(sizeRect, sizeRect);
  port = new Serial(this, "COM63", 115200);
  port.bufferUntil('\n');  
//  rgb = new int[3];
  noStroke();
  ellipseMode(CENTER);
}

void draw()
{
  background(r,g,b);
  
  fill(r, 0, 0);
  ellipse(width/4, height/2, height/4, height/4);
  
  fill(0, g, 0);
  ellipse(width/2, height/2, height/4, height/4);
  
  fill(0, 0, b);
  ellipse(2*width/4, height/2, height/4, height/4);
  
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


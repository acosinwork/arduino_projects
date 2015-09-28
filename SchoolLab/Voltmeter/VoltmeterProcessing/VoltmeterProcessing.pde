
import processing.serial.*;
Serial port;
float nextValue = 0;
int sizeRect = 1000;

String s;

FloatList oscill;

void setup()
{
  size(1000, 1000);
  port = new Serial(this, "COM10", 115200); //
  port.bufferUntil('\n');  
  oscill = new FloatList();
  s = new String("no data");
  //  textSize(18);
}

void draw()
{
  background(0);
  stroke(255);

  for (int i = 1; i< oscill.size(); ++i) {
    line(i-1, 1000-oscill.get(i-1), i, 1000-oscill.get(i));
  }

  text(s, 10, 30);
}

void serialEvent (Serial port)
{
  try {
    s = port.readStringUntil('\n');

    //  s= nextValue + " V";
    
    nextValue = float(s);


    if (!(oscill.size()<sizeRect)) {
      oscill.remove(0);
    }

    oscill.append(nextValue);
  } 
  catch (Exception e) {
    println("Connection...");
  }
}

void stop() {
  port.clear();
  port.stop();
} 
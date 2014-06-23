#define FAST 255
#define SLOW 60
#define CORR 4
#define COMM 10

int lspd = 0;
int rspd = 0;

String test, temp;
int a = 0, b = 0, c = 0, d = 0, e = 0;
int x = 0, y = 0, z = 0;
char comm;
boolean mode = false;

void setup()
{
    pinMode(4,OUTPUT);
    pinMode(5, OUTPUT);

    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    
    Serial.begin(9600);

}
void loop()
{
 
    if (Serial.available() > 0) 
    {
      char incomingChar = Serial.read();
  
      if (incomingChar != 'e' ) 
      {
        test += incomingChar;
      } 
      else 
      {
        test += 'e';
        a = test.indexOf('a');
        b = test.indexOf('b');
        c = test.indexOf('c');
        d = test.indexOf('d');
        e = test.indexOf('e');
        temp = test.substring(a+1,b);
        x = temp.toInt();
        temp = test.substring(b+1,c);
        y = temp.toInt();
        temp = test.substring(c+1,d);
        z = temp.toInt();
        temp = test.substring(d+1,e);
        comm = temp[0];
        test = "";
      }
      if(comm == 'A')
        mode = !mode;

      int sign = 0;
      if(-x + CORR > 0)
        sign = 1;
      else
        sign = -1;
      int lcomm = -x + CORR + sign*y;
      int rcomm = -x + CORR - sign*y;
      
      lcomm = constrain(lcomm, -COMM, COMM);
      rcomm = constrain(rcomm, -COMM, COMM);
      lspd = map(lcomm, -COMM, COMM, -FAST, FAST);
      rspd = map(rcomm, -COMM, COMM, -FAST, FAST);
      
      if(mode)
        drive(lspd,rspd);
      else
        drive(0,0);
  }
  
}


void drive(int speedLeft,int speedRight){
  if(speedLeft>0){
    digitalWrite(4,1);
  }else{
    digitalWrite(4,0);
  }

    if (speedRight>0)
        digitalWrite(7,1);
    else
        digitalWrite(7,0);


    analogWrite(5,abs(speedLeft));
    analogWrite(6,abs(speedRight));
} 



#define ROW 4
#define COL 3
int row[ROW] = {
  4,
  5,
  6,
  7
};

int col[COL] = {
  8,
  9,
  10,
};

char button[][ROW] ={
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*','0','#'}
};

void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < COL; ++i)
    pinMode(col[i], INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < ROW; ++i) {
   // delay(500);
    for (int y=0; y < ROW; ++y)
      pinMode(row[y], INPUT);

    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], LOW);

    for (int curCol = 0; curCol < COL; ++curCol) {
      if (!digitalRead(col[curCol]))
        Serial.println(button[ROW-i-1][COL-1-curCol]);
    }
  }




}

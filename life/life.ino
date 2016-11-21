uint8_t ROWS[] = {
  //6, 7, 8, 9, 10, 11, 12, 13
  10, 7, 12, 6, A3, 13, 3, A0
};


uint8_t COLUMNS[] = {
  2, 8, 9, A1, 11, A2, 4, 5

};

//gloabal variables
bool started = false;
bool field[8][8] = {};
bool nextField[8][8] = {};
unsigned long lastMillis = 0;

void setup() {
  // put your setup code here, to run once:
    randomSeed(analogRead(A5));

  for (int i = 0; i < 8; i++) {
    pinMode(ROWS[i], OUTPUT);
    pinMode(COLUMNS[i], OUTPUT);
    digitalWrite(COLUMNS[i], HIGH);
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      field[i][j] = random(2);
    }
  }
}

void loop() {
  

  //calculate next field
  if (millis() - 1000 > lastMillis) { //delay each iteration by 1000 milliseconds
    fillNextField();
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        field[y][x] = nextField[y][x];
      }
    }
    lastMillis = millis();

    //refresh the 8x8 LED matrix
  }
      refreshScreen();
      
}
void fillNextField() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      nextField[y][x] = getCellsNextState(x, y);
    }
  }
}

bool getCellsNextState(int x, int y) {
  bool shouldBeAlive = false;
  shouldBeAlive |= (!field[y][x] && countAliveNeighbours(x, y) == 3);
  shouldBeAlive |= (field[y][x] && countAliveNeighbours(x, y) == 3);
  shouldBeAlive |= (field[y][x] && countAliveNeighbours(x, y) == 2);
  return shouldBeAlive;
}

// counts the alive neighbours this step
int countAliveNeighbours(int x, int y) {
  int retVal = 0;
  for (int row = y - 1; row <= y + 1; row++) {
    for (int col = x - 1; col <= x + 1; col++) {
      if ((row == y && col == x) || (row < 0) || (row > 7) || (col < 0) || (col > 7)) continue;
      if (field[row][col]) retVal++;
    }
  }
  return retVal;
}

//refreshes the 8x8 LED matrix
void refreshScreen() {
 
  // iterate over the rows (anodes):
  for (int row = 0; row < 8; row++) {
    // take the row pin (anode) high:
    digitalWrite(ROWS[row], HIGH);
    // iterate over the cols (cathodes):
    for (int col = 0; col < 8; col++) {
    //  delay(1);
      // get the state of the current pixel;
      int thisPixel = field[row][col];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(COLUMNS[col], thisPixel ? LOW : HIGH);
      // turn the pixel off:
      if (thisPixel) {
        digitalWrite(COLUMNS[col], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(ROWS[row], LOW);
  }
}

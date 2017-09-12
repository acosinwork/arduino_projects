#include "Wire.h"
#include "is31fl3728.h"

IS32FL3728_TWI d;

int8_t b = 0;
int8_t bd = +1;
const int8_t N_CL = 32;

const uint8_t cl[N_CL] = 
{
 ROW_CURRENT_05MA, ROW_CURRENT_10MA, ROW_CURRENT_15MA, ROW_CURRENT_20MA,
 ROW_CURRENT_25MA, ROW_CURRENT_30MA, ROW_CURRENT_35MA, ROW_CURRENT_40MA,
 ROW_CURRENT_45MA, ROW_CURRENT_50MA, ROW_CURRENT_55MA, ROW_CURRENT_60MA,
 ROW_CURRENT_65MA, ROW_CURRENT_70MA, ROW_CURRENT_75MA, ROW_CURRENT_XXMA
};

void setup()
{
 d.begin(0);
 d.fb().clear();
 d.updateDisplay();
}

void loop()
{
 d.setCurrentLimit(cl[b]);
 b += bd;
 if (b>=(N_CL-1)) bd = -1;
 if (b<=0) bd = +1;
 //
 d.fb().setPixel(random(8),random(8));
 d.fb().clearPixel(random(8),random(8));
 d.updateDisplay();
 delay(50);
}

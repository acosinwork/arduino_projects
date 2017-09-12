#include "Wire.h"
#include "is31fl3728.h"

IS32FL3728_TWI d;

void setup()
{
 d.begin(0);
 d.fb().clear();
 d.updateDisplay();
 d.enableAudioInput();
 d.setAudioGain(AUDIO_GAIN_0DB);
 d.enableEqualizer();
}

void loop()
{
 //
}

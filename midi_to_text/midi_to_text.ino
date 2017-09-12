#include <MIDI.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1

//MIDI_CREATE_DEFAULT_INSTANCE();
    MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,    myMidi);


// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// http://arduinomidilib.fortyseveneffects.com/a00022.html

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  Serial.write(pitch);
  // Do whatever you want when a note is pressed.

  // Try to keep your callbacks short (no delays ect)
  // otherwise it would slow down the loop() and have a bad impact
  // on real-time performance.

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{

}

// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  myMidi.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

  // Do the same for NoteOffs
  myMidi.setHandleNoteOff(handleNoteOff);

  // Initiate MIDI communications, listen to all channels
  myMidi.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
  // Call MIDI.read the fastest you can for real-time performance.
  myMidi.read();
}

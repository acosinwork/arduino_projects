#include <MIDI.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            11

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      49

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//MIDI_CREATE_DEFAULT_INSTANCE();
    MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,    myMidi);
int r, g, b, pitchPot;

void f_HSV_to_RGB(int hue, int sat, int val)
{
  int base;
  if (sat == 0) {
    r = val;
    g = val;
    b = val;
  } else {
    base = ((255 - sat) * val) >> 8;
    switch (hue / 60) {
      case 0: {
          r = val;
          g = (((val - base) * hue) / 60) + base;
          b = base;
          break;
        }
      case 1: {
          r = (((val - base) * (60 - (hue % 60))) / 60) + base;
          g = val;
          b = base;
          break;
        }
      case 2: {
          r = base;
          g = val;
          b = (((val - base) * (hue % 60)) / 60) + base;
          break;
        }
      case 3: {
          r = base;
          g = (((val - base) * (60 - (hue % 60))) / 60) + base;
          b = val;
          break;
        }
      case 4: {
          r = (((val - base) * (hue % 60)) / 60) + base;
          g = base;
          b = val;
          break;
        }
      case 5: {
          r = val;
          g = base;
          b = (((val - base) * (60 - (hue % 60))) / 60) + base;
          break;
        }
      case 6: {
          r = val;
          g = 0;
          b = base;
          break;
        }
    }
  }
}

// -----------------------------------------------------------------------------

// This function will be automatically called when a NoteOn is received.
// It must be a void-returning function with the correct parameters,
// see documentation here:
// http://arduinomidilib.fortyseveneffects.com/a00022.html

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
 // Serial.println(pitch);
  // Do whatever you want when a note is pressed.

  // Try to keep your callbacks short (no delays ect)
  // otherwise it would slow down the loop() and have a bad impact
  // on real-time performance.

  f_HSV_to_RGB(velocity * 2, 255, 255);
  
  pixels.setPixelColor(pitch-pitchPot, pixels.Color(r, g, b)); // Moderately bright green color.

  pixels.show(); // This sends the updated pixel color to the hardware.

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    pixels.setPixelColor(pitch-pitchPot, pixels.Color(0,0,0)); // Moderately bright green color.

  pixels.show(); // This sends the updated pixel color to the hardware.

  // Do something when the note is released.
  // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
}

// -----------------------------------------------------------------------------

void setup()
{
 // Serial.begin(115200);
  
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
  
  // Connect the handleNoteOn function to the library,
  // so it is called upon reception of a NoteOn.
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
pitchPot = analogRead(A4)>>3;
  // There is no need to check if there are messages incoming
  // if they are bound to a Callback function.
  // The attached method will be called automatically
  // when the corresponding message has been received.
}

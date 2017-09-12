/*Nyan Cat
  with Bass
  Uses Arduino tone library pitches.h [http://arduino.cc/en/Tutorial/tone ]
       by electricmango
  https://electricmango.github.io

  Thanks to Sean for posting the sheet music on
  http://junket.tumblr.com/post/4776023022/heres-the-nyan-cat-sheet-music

  Version 1.0.3
  -------------------------------------------------------------------------
  Nyan Cat with Bass is licensed under the

  Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)

  You are free to:
  Share — copy and redistribute the material in any medium or format
  Adapt — remix, transform, and build upon the material
  for any purpose, even commercially.
  The licensor cannot revoke these freedoms as long as you follow the license terms.
  Under the following terms:
  Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
  ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
  No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
  The full license is available at https://creativecommons.org/licenses/by-sa/4.0/legalcode

  Copyright (c) 2012 ~ 2015 electricmango
  -------------------------------------------------------------------------
  Nyan_Cat.ino
  -------------------------------------------------------------------------

  Edit by Amperka.ru, add button and LEDs in 2016
  
*/

#include "pitches.h"

#define LED_R 4
#define LED_Y 10
#define LED_G 9
#define LED_B 3
#define BUZZER 2
#define BUTTON 8


//notes in the melody:
int melody[] = {
  NOTE_DS5, NOTE_E5, NOTE_FS5, 0, NOTE_B5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_DS6, NOTE_E6, NOTE_DS6, NOTE_AS5, NOTE_B5, 0,
  NOTE_FS5, 0, NOTE_DS5, NOTE_E5, NOTE_FS5, 0, NOTE_B5, NOTE_CS6, NOTE_AS5, NOTE_B5, NOTE_CS6, NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_CS6,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_B3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_AS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_B3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_AS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_CS4,
};

// note durations: 4 = quarter note, 8 = eighth note, etc
int noteDurations[] = {
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  16, 16, 16, 16, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 8, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8,
  8, 8, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 8, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8, 8,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 8, 8,
};

bool toggleLeds = false;

void setup() {
  //iterate over the notes of the melody:
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

}

void loop() {
  if (checkClick())
    nya();

  delay(10);
}

void nya()
{
  int noteCount = sizeof(melody) / sizeof(int);

  for (int thisNote = 0; thisNote < 16; thisNote++) {

//    if (checkClick())
//      return;


    if (toggleLeds = !toggleLeds) {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_Y, LOW);
      digitalWrite(LED_B, LOW);
    } else {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_Y, HIGH);
      digitalWrite(LED_B, HIGH);
    }

    /*
      to calculate the note duration, take one second divided by the note type.
      e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    */
    noTone(BUZZER); //stop the tone playing:

    int noteDuration = 1200 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);

    /*
      to distinguish the notes, set a minimum time between them.
      the note's duration + 30% seems to work well:
    */
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

  }
}

bool checkClick()
{
  return true;
  static bool buttonWasUp = true;
  bool result = false;
  bool buttonIsUp = digitalRead(BUTTON);
  if (buttonWasUp && !buttonIsUp) {
    result = true;
  }
  buttonWasUp = buttonIsUp;
  return result;
}


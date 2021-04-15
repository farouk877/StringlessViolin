/*
  Melody
  Plays a melody
  circuit:
  - 8 ohm speaker on digital pin 8
  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe
  This example code is in the public domain.
  http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"
#include "Volume.h" // Include the Volume library

Volume vol; // Plug your speaker into the default pin for your board type:
            // https://github.com/connornishijima/arduino-volume1#supported-pins
            
int speakerpin = 4;

// notes in the melody:
// main:    G3,   D4,   A4,   E5
// fret 1:  AS3,  F4,   C4,   G5
// fret 2:  CS4,  GS4,  DS5,  AS5
// fret 3:  E4,   B4,   FS5,  CS6

int melody[] = {  NOTE_G3,  NOTE_D4,  NOTE_A4,  NOTE_E5,
                  NOTE_AS3, NOTE_F4,  NOTE_C4,  NOTE_G5,
                  NOTE_CS4, NOTE_GS4, NOTE_DS5, NOTE_AS5,
                  NOTE_E4,  NOTE_B4,  NOTE_FS5, NOTE_CS6  };

// note durations: 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

void setup() {
  vol.begin(); // After calling this, delay() and delayMicroseconds will no longer work
               // correctly! Instead, use vol.delay() and vol.delayMicroseconds() for
               // the correct timing
  vol.setMasterVolume(1.00); // Self-explanatory enough, right? Try lowering this value if the speaker is too loud! (0.00 - 1.00)
  vol.delay(500);
}

void loop() {
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 16; thisNote++) {
      
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      
      // the volume (v) is between 0-255
      int v = 100;
      vol.tone(melody[thisNote], v);
      
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      vol.delay(pauseBetweenNotes);
      
    }
}

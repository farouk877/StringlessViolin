#include <Wire.h>

#include "pitches.h"
#include "Volume.h" // Include the Volume library

#define THRESHOLD 4000
#define VOLUME_DELAY 100

Volume vol; // Plug your speaker into the default pin for your board type:
            // https://github.com/connornishijima/arduino-volume1#supported-pins
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

char tmp_str[7]; // temporary variable used in convert function


char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

int speakerpin = 4;

// NATURAL VIOLIN NOTES (HARD TO PLAY WITH OUR 4 FRET PROGRESSION) 
// melody[fret][string] 
//int melody[8][4] = {{NOTE_G3,  NOTE_D4,  NOTE_A4,  NOTE_E5},
//                    {NOTE_GS3, NOTE_DS4, NOTE_AS4, NOTE_F5},
//                    {NOTE_A3,  NOTE_E4,  NOTE_B4,  NOTE_FS5},
//                    {NOTE_AS3, NOTE_F4,  NOTE_C5,  NOTE_G5},
//                    {NOTE_B3,  NOTE_FS4, NOTE_CS5, NOTE_GS5},
//                    {NOTE_C4,  NOTE_G4,  NOTE_D5,  NOTE_A5},
//                    {NOTE_CS4, NOTE_GS4, NOTE_DS5, NOTE_AS5},
//                    {NOTE_D4,  NOTE_A4,  NOTE_E5,  NOTE_B6}
//                   };


// Notes done this way to avoid string combination changes for deltas (B-C or E-F changing the deltas on strings)
// Repeat notes fill in nonexistent BS and ES gaps (EASIER TO PLAY)
int melody[8][4] = {
                    {NOTE_G3,  NOTE_D4,  NOTE_A4,  NOTE_E5},
                    {NOTE_GS3, NOTE_DS4, NOTE_AS4, NOTE_F5},
                    {NOTE_A3,  NOTE_E4,  NOTE_B4,  NOTE_FS5},
                    {NOTE_AS3, NOTE_E4,  NOTE_B4,  NOTE_G5},
                    {NOTE_B3,  NOTE_F4,  NOTE_C5,  NOTE_GS5},
                    {NOTE_B3,  NOTE_FS4, NOTE_CS5, NOTE_A5},
                    {NOTE_C4,  NOTE_G4,  NOTE_D5,  NOTE_AS5},
                    {NOTE_CS4, NOTE_GS4, NOTE_DS5, NOTE_B6}
//                    {NOTE_D4,  NOTE_A4,  NOTE_E5,  NOTE_B6}
                   };

// frets will be pairs 000, 111, 100, 110, 010, 011, 001, 101
int fretIndexes[8] = {0, 2, 4, 3, 6, 7, 5, 1};

// note durations: 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

int X_pin = A1;
int Y_pin = A0;

int fret1 = A2;
int fret2 = A3;
int fret3 = A4;

void setup() {
  Serial.begin(9600);
  vol.begin(); // After calling this, delay() and delayMicroseconds will no longer work
  vol.setMasterVolume(1.00); // Self-explanatory enough, right? Try lowering this value if the speaker is too loud! (0.00 - 1.00)
  vol.delay(500);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);

  pinMode(fret1, INPUT);
  pinMode(fret2, INPUT);
  pinMode(fret3, INPUT);
}

void loop() {
  // ---------- bow ----------
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers

  // Deconstruct message
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  // Calculate string
  int angle = accelerometer_y;
  int string = 0;
  if (angle < -THRESHOLD) {
    string = 0;
  } else if (angle < 0) {
    string = 1;
  } else if (angle < THRESHOLD) {
    string = 2;
  } else {
    string = 3;
  }

  // ----------- fretting -----------
  int fret1Val = abs(digitalRead(fret1)-1);
  int fret2Val = abs(digitalRead(fret2)-1);
  int fret3Val = abs(digitalRead(fret3)-1);
  int fretVal = fretIndexes[fret1Val + 2 * fret2Val + 4 * fret3Val];
  
  // --------------- Volume --------------------
  double x = analogRead(X_pin) - 507;
  double y = analogRead(Y_pin) - 505;
  
  int volume = int(sqrt((x * x) + (y * y)) / 731 * 255);
  if (volume < 5) volume = 0;
//    Serial.print(fret1Val);
//    Serial.print(", ");
//    Serial.print(fret2Val);
//    Serial.print(", ");
//    Serial.print(fret3Val);
//    Serial.print(", ");
//    Serial.print(string);
//    Serial.print(", ");
//    Serial.print(fretVal);
//    Serial.print(", ");
//    Serial.println(volume);
  
  // the volume (v) is between 0-255
  vol.tone(melody[fretVal][string], volume);
  
  // to distinguish the notes, set a minimum time between them.
  vol.delay(VOLUME_DELAY);
}

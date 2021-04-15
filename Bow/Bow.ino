#include <MPU6050_tockn.h>
#include <Wire.h>

#define THRESHOLD 35

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(0.06, 3.10, -1.89);
}

void loop() {
  mpu6050.update();
  int angle = mpu6050.getAngleX();
  // > 22.5, 0 - 22.5, 
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
  Serial.println(string);
  delay(1);
}

#include <MPU6050_tockn.h>
#include <Wire.h>

#define THRESHOLD 35

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(0.08, 3.14, -1.83);
}

//void loop() {
//  mpu6050.update();
//  Serial.print("angleX : ");
//  Serial.print(mpu6050.getAngleX());
//  Serial.print("\tangleY : ");
//  Serial.print(mpu6050.getAngleY());
//  Serial.print("\tangleZ : ");
//  Serial.println(mpu6050.getAngleZ());
//}

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

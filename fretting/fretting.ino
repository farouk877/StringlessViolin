int inputpin1 = A2;
int inputpin2 = A3;
int inputpin3 = A4;

void setup() {
  Serial.begin(9600);
  pinMode(inputpin1, INPUT);
  pinMode(inputpin2, INPUT);
  pinMode(inputpin3, INPUT);
}

void loop() {
  // read the values of the touch sensors
  int value1 = digitalRead(inputpin1);
  int value2 = digitalRead(inputpin2);
  int value3 = digitalRead(inputpin3);

  // configure logic if the particuar fret is pressed
  if (value1 == 0) {
    Serial.print("Fret 1 Pressed --- ");

  }
  else if (value2 == 0) {
    Serial.print("Fret 2 Pressed --- ");
  }
  else if (value3 == 0) {
    Serial.print("Fret 3 Pressed --- ");
  } 
  Serial.println("");

  // set arbitrary delay for readability
  delay(100);
}

int X_pin = A1; 
int Y_pin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(X_pin, INPUT);
  pinMode(Y_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  double x = analogRead(X_pin) - 507;
  double y = analogRead(Y_pin) - 505;

  Serial.print(x);
//  Serial.print(analogRead(X_pin));
  Serial.print(", ");
  Serial.print(y);
//  Serial.print(analogRead(Y_pin));
  Serial.print(", ");

  double magnitude = sqrt((x * x) + (y * y));
  Serial.println(magnitude);
}

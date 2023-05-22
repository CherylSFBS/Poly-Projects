int total = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("Sum of ");
}

void loop() {

  for (int x = 5; x < 221;) {
    x = x + 2;
    Serial.print(x);
    if (x < 220) {
      Serial.print(", ");
    }
    delay(50);
    total = total + x;
  }
  Serial.print(" is ");
  Serial.print(total);
  while (1);

}

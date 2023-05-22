
long total = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int num = 4; num < 491;) {
    Serial.print(num);
    total = total + num;
    if (num < 490) {
      Serial.print(", ");
    }
    num = num + 2;
    delay(10);

  }
  Serial.println("");
  Serial.print("Total ");
  Serial.print(total);
  while (1);
}

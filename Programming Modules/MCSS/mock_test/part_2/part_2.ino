void setup() {
  Serial.begin(19200);
  Serial.println("Hello World!");

}

void loop() {
  for (int x = 0; x < 10; x++) {
    Serial.print("Print ");
    Serial.println(x);
    delay(1000);
  }
  Serial.println(x);
  while (1);

}

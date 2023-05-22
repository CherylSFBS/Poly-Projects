int cnt = 0;
void setup() {
  Serial.begin(4800);
  Serial.println("25MCSS Practical Test");
  
}

void loop() {
  cnt++;
  Serial.print("hello");
  Serial.write(0x21);
  Serial.print(cnt);
  Serial.println(" times");
  delay(2000);
}

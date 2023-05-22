int count = 0;
int countcheck = 1;
int resetcheck = 1;

void setup() {
  Serial.begin(4800);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  pinMode(A0, INPUT);
  Serial.println("Counter system starts!");
}

void loop() {
  int lux = analogRead(A0);
  //Serial.println(lux);
  if (lux < 375) {
    digitalWrite(3, HIGH);
    if (countcheck == 1) {
      count = count + 1;
      countcheck = 0;
      Serial.print("Count = ");
      Serial.println(count);
    }
  }
  if (lux > 480) {
    digitalWrite(3, LOW);
    countcheck = 1;
  }

  int button = digitalRead(2);
  //Serial.println(button);
  if (button == 0) {
    if (resetcheck == 1) {
      count = 0;
      resetcheck = 0;
      Serial.println("Count initialize to 0");
    }
  }
  if (button == 1){
    resetcheck = 1;
  }

}

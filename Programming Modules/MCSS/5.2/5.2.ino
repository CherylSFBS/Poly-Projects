void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
}

void loop()
{
  int lightread = analogRead(A0);
  float lux = lightread * (5.0 / 1023.0);
  Serial.println(lux);
  if(lux > 3){
    digitalWrite(13, LOW);
  }
  if(lux < 3){
    digitalWrite(13, HIGH);
  }
  delay(200);
}

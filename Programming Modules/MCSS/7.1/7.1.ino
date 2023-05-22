void setup()
{
  pinMode(3, OUTPUT); //pwm at pin 3
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}
void loop()
{
  int lightread = analogRead(A0);
  Serial.println(lightread);
  if (lightread < 600) {
    analogWrite(9, 255);
    analogWrite(3, 127); //50%
  }
  else {
    analogWrite(3, 255); //100%
    analogWrite(9, 127);
  }
}

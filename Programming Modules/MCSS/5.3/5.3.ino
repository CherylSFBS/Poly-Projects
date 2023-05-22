void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  int tempread = analogRead(A0);
  float voltage = tempread/1024.0 * 5.0;
  float temperature = (voltage - 0.5) * 100.0;
  Serial.print(temperature);
  Serial.println("°C");
  delay(200);
}

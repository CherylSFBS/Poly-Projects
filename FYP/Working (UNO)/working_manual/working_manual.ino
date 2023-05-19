// ONLY FOR MANUAL TESTING/CALIBRATION


void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT); //FRONT Conveyor sensor
  digitalWrite(2, HIGH);
  pinMode(3, INPUT); //BACK Conveyor sensor
  digitalWrite(3, HIGH);
  pinMode(4, OUTPUT); //Scissor Lift direction control
  pinMode(5, OUTPUT); //Scissor lift power state control
  pinMode(6, OUTPUT); //Conveyor motor, HIGH = turn on
  pinMode(7, INPUT); //Scissor Lift Stop_PB
  pinMode(8, INPUT); //Limit switch at bottom
  digitalWrite(8, HIGH);
  pinMode(9, INPUT); //Limit switch at top
  digitalWrite(9, HIGH);
  pinMode(10, INPUT); //Scissor Lift Sensor, I/O, Highest
  pinMode(11, INPUT); //Scissor Lift Sensor, Depot, Middle
  pinMode(12, INPUT); //Scissor Lift Sensor, Transit, Lowest
}

int check = 0;
int control = 0;

void loop()
{
  //DIGITALWRITE 4 SCISSOR CONTROL, HIGH = UP, LOW = DOWN
  bool bottlefront = digitalRead(2); // LOW = CAN DETECT, HIGH = NO DETECT
  bool bottleback = digitalRead(3); // LOW = CAN DETECT, HIGH = NO DETECT
  bool UU = digitalRead(8);
  bool OU = digitalRead(9);
  bool onoff = digitalRead(11);
  bool updown = digitalRead(12);
  /*bool hinserter = digitalRead(10);
    bool hstorage = digitalRead(11);
    bool hmove = digitalRead(12);
  */
  /*
    digitalWrite(4, LOW); // go down
    digitalWrite(4, HIGH); // go up
    digitalWrite(5, HIGH);*/

  if (Serial.available() > 0) {
    control = Serial.read();
    }

    if (control == 49) {
    digitalWrite(5, HIGH); 
    }
    else if (control == 50) {
    digitalWrite(5, LOW);
    }
    else if(control == 51){
      digitalWrite(4, LOW); //up
    }
    else if(control == 52){
      digitalWrite(4, HIGH); //down
    }
    else if(control == 53){ //5
      digitalWrite(6, HIGH);
    }
    else if(control == 54){
      digitalWrite(6, LOW);
    }
  /*

    //TRANSFER
    if (Serial.available() > 0) {
      control = Serial.read();
    }

    if (control == 49) { //1
      digitalWrite(5, HIGH);
    }

    else if (control == 50) { //2
      digitalWrite(5, LOW);
    }

    if (OU == LOW) { //Upper limiter
      check = 1;
      digitalWrite(5, LOW);
      if (control == 51) {
        digitalWrite(5, HIGH);
        digitalWrite(4, HIGH);
        while (OU == LOW) {
          OU = digitalRead(9);
          if (OU == HIGH) {
            break;
          }
        }
        delay(300);
        if (control == 50) {
          digitalWrite(5, LOW);
        }
        check = 0;
      }
    }

    else if (control == 51 && OU == HIGH && check == 0) { //3
      digitalWrite(4, LOW);
    }


    if (UU == LOW) { //Lower limiter
      check = 1;
      digitalWrite(5, LOW);
      if (control == 52) {
        digitalWrite(5, HIGH);
        digitalWrite(4, HIGH);
        delay(500);
        digitalWrite(5, LOW);
        check = 0;
      }
    }

    else if (control == 52 && UU == HIGH && check == 0) { //4
      digitalWrite(4, HIGH);
    }

    if (bottlefront == LOW) {
      digitalWrite(6, HIGH);
    }
    else {
      digitalWrite(6, LOW);
    }
  */
  /*float test = analogRead(A0);
    ADCFilter.Filter(test);
    Serial.print(test * 5 / 1023);
    Serial.print(", left = ");
    Serial.print(onoff);
    Serial.print(", right = ");
    Serial.println(updown);
    Serial.print("1 ON, 2 OFF, 3 UP, 4 DOWN, check = ");
    Serial.print(check);
    Serial.print(", Conveyor Sensor Front = ");
    Serial.print(bottlefront);
    Serial.print(", Conveyor Sensor Back = ");
    Serial.print(bottleback);
    Serial.print(", Limit Check = ");
    Serial.print(check);
    Serial.print(", Insertion H = ");
    Serial.print(hinserter);
    Serial.print(", Storage H = ");
    Serial.print(hstorage);
    Serial.print(", Move H = ");
    Serial.println(hmove);
  */
  /*Serial.print(onoff);
    Serial.println(check);
  */
  delay(20);

}

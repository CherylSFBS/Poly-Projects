/*
Name : Chew Tze Han
Student No.: S10203491A
Group: P2M2
Practice No.: 2.2

This program prints out execute loop() number in ascending order
*/
int i; //this defines integer i

void setup() {
  Serial.begin(9600); //this starts the serial monitor on 9600 bits
  i = 0; //this makes integer i have a value of 0
  Serial.println("execute setup()"); //this prints out execute setup() in serial monitor

}

void loop() {
  delay(10); //this causes a delay of 10 milliseconds
  Serial.print("execute loop() number "); //this prints out execute loop() number in serial monitor
  Serial.println(i++); //this prints out the value of i in serial monitor
}

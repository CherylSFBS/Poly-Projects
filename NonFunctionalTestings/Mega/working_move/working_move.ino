#include <Sabertooth.h>
#include <SoftwareSerial.h>

Sabertooth ST1(128);
Sabertooth ST2(129);
/*SoftwareSerial SWSerial(NOT_A_PIN, 18);
  Sabertooth ST1(128, SWSerial);
  Sabertooth ST2(129, SWSerial);
*/
void setup()
{
  //SWSerial.begin(9600);
  //Serial3.begin(9600);

  SabertoothTXPinSerial.begin(9600);
  Sabertooth::autobaud(SabertoothTXPinSerial);

  pinMode(4, OUTPUT); //led ?
  pinMode(5, OUTPUT); //led ?
  pinMode(10, INPUT); //Track detected
  pinMode(11, INPUT); //MARKER LEFT
  pinMode(12, INPUT); //MARKER RIGHT
  pinMode(7, INPUT); //BACK DOCKER
  pinMode(8, INPUT); //FRONT DOCKER

  ST1.setRamping(1);
  ST2.setRamping(1);
}



//That is a lot of checks

int check = 0; //Main loop prevention
int transcheck = 0; //Transfer check
int dispcheck = 0; //Display check
long dockinfo = 0; //Temp Docking check
int entrans = 0; //Enter Transfer check
int upcheck = 0; //Ascending check

bool checkrev = 0;
bool st0p = 0;
byte revmagstrip = 0;
byte stopmagstrip = 0;
bool bleenklight = 0;

void loop()
{

  bool trackdet = digitalRead(10); //HIGH = DETECTED, LOW = UNDETECTED
  bool LMARK = digitalRead(11); //HIGH = DETECTED, LOW = UNDETECTED
  bool RMARK = digitalRead(12); //HIGH = DETECTED, LOW = UNDETECTED

  float track = analogRead(A0); //avg 1.75
  track = track * 5 / 1023;

ST1.motor(1, 30);
ST1.motor(2, 30);
ST2.motor(1, 30);
ST2.motor(2, 30);
}

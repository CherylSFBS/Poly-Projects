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
  Serial3.begin(9600);

  SabertoothTXPinSerial.begin(9600);
  Sabertooth::autobaud(SabertoothTXPinSerial);

  //pinMode(4, OUTPUT); //led ?
  //pinMode(5, OUTPUT); //led ?
  pinMode(6, INPUT_PULLUP); //DOCKING_PB with internal resistor
  pinMode(10, INPUT); //Track detected
  pinMode(11, INPUT); //MARKER LEFT
  pinMode(12, INPUT); //MARKER RIGHT


  ST1.setRamping(10); //10 = fastest change
  ST2.setRamping(10);
}



//That is a lot of checks



bool checkrev = 0;
bool st0p = 0;
byte revmagstrip = 0;
bool revdock = 0;

float stabcalc;
long dettime = 0;
bool trackdetcheck = 0;
int serialcomm = 0; //move initially
int serialchecker = 0;


/*receive 49 = can go
   send 49 = started docking
   reset to 0 after sending to cancel everything
*/
void loop()
{
  if (Serial3.available() > 0) {
    serialchecker = Serial3.read(); //
  }


  if (serialchecker == 50) {
    serialcomm = 50;
  }

  bool trackdet = digitalRead(10); //HIGH = DETECTED, LOW = UNDETECTED
  bool LMARK = digitalRead(11); //HIGH = DETECTED, LOW = UNDETECTED
  bool RMARK = digitalRead(12); //HIGH = DETECTED, LOW = UNDETECTED

  /*Serial.print("Track = ");
    Serial.print(track);
    if (track < 1.50) {
    Serial.print(", Track too far right");
    }
    else if (track > 2.2) {
    Serial.print(", Track too far left");
    }
    Serial.print(", left = ");
    Serial.print(updown);
    Serial.print(", right = ");
    Serial.println(onoff);*/

  /* ST1, 1 = BOTTOM RIGHT
     ST1, 2 = BOTTOM LEFT
     ST2, 1 = TOP RIGHT
     ST2, 2 = TOP LEFT
     ST1.motor
     ST1.stop
  */

  /*if (revdock = 1 && serialcomm == 49) { //reverse and then spin 180 to move
      ST1.motor(1, -20);
      ST1.motor(2, -20);
      ST2.motor(1, -20);
      ST2.motor(2, -20);
      delay(1500);
      ST1.stop();
      ST2.stop();
      delay(500);
      goto spinnyspinny;
      }*/
      
  if (serialcomm == 50) {
    ST1.stop();
    ST2.stop();
    delay(1000);
    ST1.motor(1, -20);
    ST1.motor(2, -20);
    ST2.motor(1, -20);
    ST2.motor(2, -20);//reverse before spinning
    delay(1500);
    ST1.stop();
    ST2.stop();
    delay(1000);
    goto spinnyspinny;
  }


  while (trackdet == HIGH && revdock == 0) { //if track is detected
    //Toggle backwards / forwards


    bool dockpb = digitalRead(6); // 0 = pressed
    trackdet = digitalRead(10); //HIGH = DETECTED, LOW = UNDETECTED
    LMARK = digitalRead(11); //HIGH = DETECTED, LOW = UNDETECTED
    RMARK = digitalRead(12); //HIGH = DETECTED, LOW = UNDETECTED

    if (trackdet == LOW) { //standard stop if fall out of track
      ST1.stop();
      ST2.stop();
      break;
    }

    /*if (dockpb == 0) { //temp dock button test
      ST1.stop();
      ST2.stop();
      delay(1000);
      ST1.motor(1, -20);
      ST1.motor(2, -20);
      ST2.motor(1, -20);
      ST2.motor(2, -20);//reverse before spinning
      delay(1500);
      ST1.stop();
      ST2.stop();
      delay(1000);
      goto spinnyspinny;
      }*/

    if (dockpb == 0) { //if docking button is pressed
      ST1.stop();
      ST2.stop();
      revdock = 1;
      serialcomm = 51;
      Serial3.write(49);
      break;
    }




    /*  if (LMARK == HIGH && revmagstrip == 0 && st0p == 0) { //LEFT MARKER = TOGGLE REVERSE
        checkrev = 1;
        revmagstrip = 1;
      }
      else if (LMARK == LOW && revmagstrip == 1 && st0p == 0) {
        revmagstrip = 2;
      }

      else if (LMARK == HIGH && revmagstrip == 2 && st0p == 0) {
        checkrev = 0;
        revmagstrip = 3;
      }
      else if (LMARK == LOW && revmagstrip == 3 && st0p == 0) {
        revmagstrip = 0;
      }
    */
    /* int fintransfer;
      if (Serial.available() > 0) {
       fintransfer = Serial.read();
      }
      if (RMARK == HIGH && stopmagstrip == 0) { //RIGHT MARKER = STOP FOR DOCKING
       st0p = 1;
       stopmagstrip = 1;
       Serial3.write(1); //communicate with the mounted arduino to start scissor lift moving process
      }
      else if (fintransfer == 1 && stopmagstrip == 1) {
       stopmagstrip = 2;
       st0p = 0;
      }
      else if (RMARK == LOW && stopmagstrip == 2) {
       stopmagstrip = 0;
      }/*
       else if (RMARK == LOW && stopmagstrip == 3) {
       stopmagstrip = 0;
       }*/


    /*    if (trackdet == LOW) {
              dettime = millis();
              while ((millis() - dettime) < 1000) {
                if ((millis() - dettime) > 1000) {
                  ST1.stop();
                  ST2.stop();
                  break;
                }
              }
              break;
            }*/

    /*if (trackdet == HIGH) {
      dettime = 0;
      trackdetcheck = 0;
      }
      else if (((millis() - dettime) > 1000) && trackdetcheck == 1) {
      ST1.stop();
      ST2.stop();
      trackdetcheck = 0;
      dettime = 0;
      break;
      }*/



    /*
        //long bleenkcount = millis();
        if ((millis() % 250) <= 124) {
          bleenklight = 1;
        }
        else if ((millis() % 250) >= 125) {
          bleenklight = 0;
        }
    */

    //fwdstab
    //make 44.1175 into <30 (ex. 25)
    //make 31 into as close <30 (ex. 29)
    //yawstab
    //make 8.8235 into as far >yawstab (ex. 19)
    //make 29 into into as close >yawstab (ex. 16)
    //the further the value of stabiliser from 30, the weaker fwdstaab, the stronger yawstab



    /*
        float revstabcalc1 = stabiliser - 26;
        float revstabcalc2 = pow(revstabcalc1, 3);
        float revstabcalc3 = -0.002 * revstabcalc2;
        float revstabcalc = revstabcalc3 + 30;

        float revfwdstab = 60 + revstabcalc;
        float revyawstab = revstabcalc;
    */


    /*float stabcalc1 = stabiliser * 1.50 - 39; 
        float stabcalc2 = pow(stabcalc1, 3);
        float stabcalc3 = (1.00 / 800.00) * stabcalc2;
        float stabcalc = stabcalc3 + 30;

        float fwdstab = 60 - stabcalc;
        float yawstab = stabcalc;
        float revfwdstab = -fwdstab;
        float revyawstab = -yawstab;*/
    // (1/800)(stabiliser - 30)^3 + 30

    /*
      float stabcalc1 = stabiliser * 1 - 30; //slight offset
      float stabcalc2 = pow(stabcalc1, 3);
      float stabcalc3 = (1.00 / 800.00) * stabcalc2;
      float stabcalc = stabcalc3 + 30;

      float fwdstab = 60 - stabcalc;
      float yawstab = stabcalc;*/

    /*if (stabiliser < 14.07) { //setting min
      stabiliser = 14.07;
      }
      else if (stabiliser > 42.93) { //setting max
      stabiliser = 42.93;
      }*/


    /* float stabcalc1 = stabiliser * 2.5 - 75;
      float stabcalc2 = pow(stabcalc1, 3);
      float stabcalc3 = (1.00 / 800.00) * stabcalc2;
      float stabcalc = stabcalc3 + 30;


      float fwdstab = 60 - stabcalc;
      float yawstab = stabcalc;*/
    /*if (fwdstab >= 60) {
          fwdstab = 30;
          yawstab = -30;
        }

        else if (yawstab >= 60) {
          yawstab = 30;
          fwdstab = -30;
        }*/

    float track = analogRead(A0); //avg 1.75
    track = (track * 5 / 1023) - 0.17;

    if (track <= 0.5) {
      track = 0.5;
    }

    float stabiliser = track * 17.647; //at 1.7 will be 30, at 0.5 will be 8.8235, at 2.5 will be 44.1175


    float stabcalc1 = stabiliser * 1 - 26; //working to move in a straight line for sensor in front DONT DELETE
    float stabcalc2 = pow(stabcalc1, 3);
    float stabcalc3 = (1.00 / 90.00) * stabcalc2;
    float stabcalc = stabcalc3 + 30;

    float fwdstab = 60 - stabcalc;
    float yawstab = stabcalc;




    /*if (fwdstab > 60) { //force spot turn when robot reaches extreme end >60
      fwdstab = 60;
      yawstab = -60;
      }

      else if (yawstab > 60) {
      fwdstab = -60;
      yawstab = 60;
      }*/
    /* float revfwdstab = -fwdstab;
      float revyawstab = -yawstab;
    */
    //-----------------------------------------------------------------------------------------
    /*if (checkrev == 0) {
      ST1.motor(1, yawstab);
      ST1.motor(2, fwdstab);
      ST2.motor(1, yawstab);
      ST2.motor(2, fwdstab);
      }*/
    /*if(stopmagstrip = 1){
      Serial.write(49);
      ST1.stop();
      ST2.stop();
      break;
      }*/
    if (checkrev == 0) {
      ST1.motor(1, fwdstab);
      ST1.motor(2, yawstab);
      ST2.motor(1, fwdstab);
      ST2.motor(2, yawstab);
    }

    else if (checkrev == 1) { //reverse sequence after detecting left marker
      delay(1000);

spinnyspinny: //docking entrance
      revdock = 0;
      ST1.motor(1, 30);
      ST1.motor(2, -30);
      ST2.motor(1, 30);
      ST2.motor(2, -30);
      delay(1000);
      float track = analogRead(A0); //avg 1.75
      track = (track * 5 / 1023) - 0.17;
      while (track < 4.0)  {//start loop
        float track = analogRead(A0); //avg 1.75
        track = (track * 5 / 1023) - 0.17;
        if (track >= 1.35 && track <= 1.65 && digitalRead(10) == HIGH) {
          checkrev = 0;
          ST1.stop();
          ST2.stop();
          delay(1000);
          serialcomm = 51;
          serialchecker = 51;
          break;
        }
      }
    }
    //Reverse testing
    /*  else if (checkrev == 1) {
        ST1.motor(1, revyawstab);
        ST1.motor(2, revfwdstab);
        ST2.motor(1, revyawstab);
        ST2.motor(2, revfwdstab);
      }*/
    /*else if (checkrev == 1) {
      ST1.motor(1, revyawstab);
      ST1.motor(2, revfwdstab);
      ST2.motor(1, revyawstab);
      ST2.motor(2, revfwdstab);
      }*/


    //IF LAST KNOWN POSITION > 2.5, TURN LEFT
    /*if (checkrev == 1) { //reverse movement
      if (track >= 1.7) {
        ST1.motor(1, -15);
        ST1.motor(2, -30);
        ST2.motor(1, -30);
        ST2.motor(2, -15);
      }

      else if (track <= 1.3) {
        ST1.motor(1, -30);
        ST1.motor(2, -15);
        ST2.motor(1, -15);
        ST2.motor(2, -30);
      }
      else {
        ST1.motor(1, 30);
        ST1.motor(2, 30);
        ST2.motor(1, 30);
        ST2.motor(2, 30);
      }
      }*/

    /* //I hate this.
      else if (checkrev == 1) {
       if (track < 1.3 && track > 1.0) {
         ST1.motor(1, 10);
         ST1.motor(2, 10);
         ST2.motor(1, 10);
         ST2.motor(2, 20);
       }
       else if (track > 1.7 && track < 2.0) {
         ST1.motor(1, 10);
         ST1.motor(2, 10);
         ST2.motor(1, 20);
         ST2.motor(2, 10);
       }
       else {
         ST1.motor(1, 20);
         ST1.motor(2, 20);
         ST2.motor(1, 20);
         ST2.motor(2, 20);
       }
      }*/

    /*  float fwdstab = 30;
      float yawstab = 15;
          if (checkrev == 0 && st0p == 0) { //standard movement
            if (track >= 1.85) {
              ST1.motor(1, 30);
              ST1.motor(2, 15);
              ST2.motor(1, 15);
              ST2.motor(2, 30);
            }
            else if (track <= 1.3) {
              ST1.motor(1, 15);
              ST1.motor(2, 30);
              ST2.motor(1, 30);
              ST2.motor(2, 15);
            }
            else {
              ST1.motor(1, 30);
              ST1.motor(2, 30);
              ST2.motor(1, 30);
              ST2.motor(2, 30);
              digitalWrite(4, HIGH);
              digitalWrite(5, LOW);
            }
          }

          if (checkrev == 1 && st0p == 0) { //reverse movement
            if (track >= 1.85) {
              ST1.motor(1, -15);
              ST1.motor(2, -30);
              ST2.motor(1, -30);
              ST2.motor(2, -15);
            }

            else if (track <= 1.3) {
              ST1.motor(1, -30);
              ST1.motor(2, -15);
              ST2.motor(1, -15);
              ST2.motor(2, -30);
            }

            else {
              ST1.motor(1, -30);
              ST1.motor(2, -30);
              ST2.motor(1, -30);
              ST2.motor(2, -30);
              digitalWrite(4, HIGH);
              digitalWrite(5, LOW);
            }
          }*/
  }
  /* else if (st0p == 1) {
     Serial3.write(1);
     ST1.stop();
     ST2.stop();
     digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
    }

    else if (st0p == 0) {
     Serial3.write(0);
    }*/




  /* //FORWARDS
    ST1.motor(1, 60);
    ST1.motor(2, 60);
    ST2.motor(1, 60);
    ST2.motor(2, 60);
    //FORWARDS

    //BACKWARDS
    ST1.motor(1, -60);
    ST1.motor(2, -60);
    ST2.motor(1, -60);
    ST2.motor(2, -60);
    //BACKWARDS

    //LEFT
    ST1.motor(1, 0);
    ST1.motor(2, 60);
    ST2.motor(1, 60);
    ST2.motor(2, 0);
    //LEFT

    //RIGHT
    ST1.motor(1, 60);
    ST1.motor(2, 0);
    ST2.motor(1, 0);
    ST2.motor(2, 60);
    //RIGHT

    //SPOT TURN LEFT
    ST1.motor(1, -60);
    ST1.motor(2, 60);
    ST2.motor(1, -60);
    ST2.motor(2, 60);
    //SPOT TURN LEFT

    //SPOT TURN RIGHT
    ST1.motor(1, 60);
    ST1.motor(2, -60);
    ST2.motor(1, 60);
    ST2.motor(2, -60);
    //SPOT TURN RIGHT
  */
}

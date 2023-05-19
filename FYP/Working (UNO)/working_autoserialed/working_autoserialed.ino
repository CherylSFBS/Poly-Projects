
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
  //pinMode(11, INPUT); //MARKER LEFT
  //pinMode(12, INPUT); //MARKER RIGHT
  pinMode(LED_BUILTIN, OUTPUT);
}


byte loadcount = 0;   //Load present


bool check = 0; //Main loop prevention
byte transcheck = 0; //Transfer check
bool dispcheck = 0; //Display check
int dockinfo = 0; //Temp Docking check
bool entrans = 0; //Enter Transfer check
bool upcheck = 0; //Ascending check
bool downcheck = 0; //Descending check
int tempcheck = 0; //Temporary testing check, delete later
bool limitcheck = 0; //Scissor lift at height limit check
int exitlimitcheck = 0; //Check for exitting limit loop
bool movcheck = 0; //Keep inside loop while moving check
bool stopmov = 0; //Latch for stop_pb
bool limitmovcheck = 0; //check for moving during estop
int serialcheck = 0;//serial check before it starts
//long limitmovtime = 0; //millis holder for moving during limit


void loop()
{

  //DIGITALWRITE 4 SCISSOR CONTROL, HIGH = UP, LOW = DOWN
  //DIGITALWRITE 5 POWER STATE CONTROL, HIGH = ON, LOW = OFF
  //bool LMARK = digitalRead(11); //HIGH = DETECTED, LOW = UNDETECTED
  //bool RMARK = digitalRead(12); //HIGH = DETECTED, LOW = UNDETECTED
  bool bottlefront = digitalRead(2); // LOW = OBJECT PRESENT, HIGH = NOTHING PRESENT
  bool bottleback = digitalRead(3); // LOW = OBJECT PRESENT, HIGH = NOTHING PRESENT
  bool UU = digitalRead(8); // LOW = REACHED LOWER LIMIT, HIGH = NOT AT LOWER LIMIT
  bool OU = digitalRead(9); // LOW = REACHED UPPER LIMIT, HIGH = NOT AT UPPER LIMIT
  bool height[3] = {digitalRead(10), digitalRead(12), digitalRead(11)}; // 1 = I/O, 2 = Depot, 3 = Transit
  bool scisspb = digitalRead(7); //Scissorlift STOP push button
  long limitmovtime;


  /*float track = analogRead(A0);
    track = track * 5 / 1023;
    Serial.print("Track = ");
    Serial.print(track);
    if (track < 1.50) {
    Serial.print(", Track too far right");
    }
    else if (track > 2.2) {
    Serial.print(", Track too far left");
    }

    Serial.print(", Left = ");
    Serial.print(LMARK);
    Serial.print(", Right = ");
    Serial.print(RMARK);*/
  /*Serial.print("Upcheck = ");
    Serial.print(upcheck);
    Serial.print(", Entrans = ");
    Serial.print(entrans);
    Serial.print(", Scissor PB = ");
    Serial.print(scisspb);
    Serial.print(", I/O height = ");
    Serial.print(height[0]);
    Serial.print(", Depot height = ");
    Serial.print(height[1]);
    Serial.print(", Transit height = ");
    Serial.print(height[2]);
    Serial.print(", dockinfo = ");
    Serial.println(dockinfo);*/
  // Sensor checking




  while (limitcheck == 1 || stopmov == 1) { //Limit Switch reached, will move back down to slightly above lower limit
    UU = digitalRead(8);
    OU = digitalRead(9);/*
    if (limitcheck == 1) {
      Serial.println("Scissor Lift has reached height limit, enter 1 to continue normal operation. 2 to lower, 3 to raise");
    }
    else if (stopmov == 1) {
      Serial.println("Scissor Lift Stop Button has been pressed, enter 1 to continue normal operation. 2 to lower, 3 to raise");
    }*/

    if (UU == HIGH) {
      delay(50);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      delay(1000);
      limitcheck = 0;
      stopmov = 0;
      break;
    }

    else if (OU == HIGH) {
      delay(50);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      while (UU == LOW) {
        if (UU == HIGH) {
          break;
        }
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      delay(1000);
      limitcheck = 0;
      stopmov = 0;
      break;
    }

    digitalWrite(5, LOW);
    check = 0;
    delay(2000);
  }


  /*if (Serial.available() > 0) {
    exitlimitcheck = Serial.read();
    }
    if (exitlimitcheck == 49) {
    Serial.println("Resuming Standard Operation");
    limitcheck = 0;
    stopmov = 0;
    exitlimitcheck = 0;
    break;
    }
    else if (exitlimitcheck == 50 && UU == HIGH) { //Manual lowering of scissor lfit for reset
    delay(50);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    limitmovtime = millis();
    while (limitmovcheck == 0) {
      UU = digitalRead(8);
      Serial.println("Scissor Lift going DOWN");
      if (UU == LOW) { //IF LOWER LIMIT REACHED
        limitmovcheck = 1;
        Serial.println("LOWER LIMIT REACHED, PLEASE MOVE UPWARDS");
        break;
      }
      else if ((millis() - limitmovtime) > 1000) { //millis - ms since program start, limitmovtime - ms since before while loop
        limitmovcheck = 1;
        break;
      }

    }
    exitlimitcheck = 0;
    limitmovcheck = 0;
    }
    else if (exitlimitcheck == 51 && OU == HIGH) { //Manual raising of scissor lift for reset
    delay(50);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    limitmovtime = millis();
    while (limitmovcheck == 0) {
      OU = digitalRead(9);
      Serial.println("Scissor Lift going UP");
      if (OU == LOW) { //IF UPPER LIMIT REACHED
        limitmovcheck = 1;
        Serial.println("LOWER LIMIT REACHED, PLEASE MOVE DOWNWARDS");
        break;
      }
      else if ((millis() - limitmovtime) > 1000) {
        limitmovcheck = 1;
        break;
      }

    }
    exitlimitcheck = 0;
    limitmovcheck = 0;
    }*/


  if (Serial.available() > 0) { //READ SERIAL
    serialcheck = Serial.read(); //needed 49 to start the process
  }

  if (serialcheck == 49)  {
    tempcheck = 49;
  }

  if ((tempcheck == 49) && check == 0) { //WHEN DETECTED THAT ROBOT IS DOCKED
    //motor stop and vehicle is docked
    delay(2000);
    check = 1;
    tempcheck = 0;
    while (upcheck == 0) {
      /*Serial.println("Enter value for Scissor Lift Movement, 1 = I/O & 2 = Depot (temp test)"); //Communicate with mega2560
        if (Serial.available() > 0) {
        dockinfo = Serial.read();
        Serial.print(dockinfo);*/
      dockinfo = 49;
      if (dockinfo == 49) { // I/O HEIGHT
        while (movcheck == 0) {
          scisspb = digitalRead(7);
          OU = digitalRead(9);
          height[0] = digitalRead(10);
          digitalWrite(4, LOW);
          digitalWrite(5, HIGH);
          /*Serial.print("dockinfo = ");
            Serial.print(dockinfo);
            Serial.print(" sciss pb = ");
            Serial.print(scisspb);
            Serial.print(", OU = ");
            Serial.print(OU);
            Serial.print(" I/O height = ");
            Serial.println(height[0]);*/
          /*if (dispcheck == 0) {
            dispcheck = 1;
            Serial.println("SCISSOR LIFT GOING UP TO I/O");
            }*/

          /*if (scisspb == 1) {
            scpbgoto:
            stopmov = 1;
            dispcheck = 0;
            dockinfo = 0;
            upcheck = 1;
            movcheck = 1;
            break;
            }*/

          for (int a = 0; height[0] == HIGH; a++) { //sensor inconsistency / centering sensor
            height[0] = digitalRead(10);
            delay(20);
            if (a > 5) {
              entrans = 1;
              dispcheck = 0;
              dockinfo = 0;
              upcheck = 1;
              movcheck = 1;
              break;
            }
          }
          /*if (OU == LOW) {
            ougoto:
            Serial.println("REACHED UPPER LIMIT, RETRACTING AND RESETTING");
            digitalWrite(5, LOW);
            delay(1000);
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            delay(1500);
            dispcheck = 0;
            dockinfo = 0;
            upcheck = 1;
            limitcheck = 1;
            movcheck = 1;
            break;
            }*/
        }
        if (upcheck == 1) {
          upcheck = 0;
          break;
        }
      }
      else if (dockinfo == 50) { // DEPOT HEIGHT
        while (movcheck == 0) {
          scisspb = digitalRead(7);
          OU = digitalRead(9);
          height[1] = digitalRead(11);
          digitalWrite(4, LOW);
          digitalWrite(5, HIGH);
          /*Serial.print("dockinfo = ");
            Serial.print(dockinfo);
            Serial.print(" sciss pb = ");
            Serial.print(scisspb);
            Serial.print(", OU = ");
            Serial.print(OU);
            Serial.print(" Depot height = ");
            Serial.println(height[1]);*/
          if (dispcheck == 0) {
            dispcheck = 1;
            //Serial.println("SCISSOR LIFT GOING UP TO DEPOT");
          }
          if (scisspb == 1) {
            //            goto scpbgoto;
          }

          for (int b = 0; height[1] == HIGH; b++) { //sensor inconsistency
            height[1] = digitalRead(11);
            delay(40);
            if (b > 5) {
              entrans = 1;
              dispcheck = 0;
              dockinfo = 0;
              upcheck = 1;
              movcheck = 1;
              break;
            }
          }
          if (OU == LOW) {
            //            goto ougoto;
            break;
          }
        }
        if (upcheck == 1) {
          upcheck = 0;
          break;
        }
      }

    }
    movcheck = 0;
    digitalWrite(5, LOW);
    //Serial.println("SCISSOR LIFT STOPPED");
    delay(1000);
    if (stopmov == 0 && limitcheck == 0) {
      check = 0;
    }
  }



  //Transferring start
  if (entrans == 1) {
    bottlefront = digitalRead(2);
    bottleback = digitalRead(3);

    if (bottlefront == LOW && bottleback == LOW) { //BOTH HAVE ITEM, UNLOAD
      loadcount = 0;
      transcheck = 1;
    }
    else if (bottlefront == LOW && bottleback == HIGH) { //FRONT HAS ITEM, BACK HAS NOTHING, UNLOAD
      loadcount = 1;
    }
    else if (bottlefront == HIGH && bottleback == LOW) { //FRONT HAS NOTHING, BACK HAS ITEM, UNLOAD
      loadcount = 2;
      transcheck = 1;
    }
    else if (bottlefront == HIGH && bottleback == HIGH) { //NOTHING, LOAD
      loadcount = 3;
      transcheck = 1;
    }

    loadcount = 4; //i hate me
    //Serial.print("Loadcount = ");
    //Serial.print(loadcount);
    //Serial.print(" Transfercheck = ");
    //Serial.println(transcheck);

    /*    track = analogRead(A0);
        track = track * 5 / 1023;
        Serial.print("Track = ");
        Serial.println(track);
    */

    //-----------------------------UNLOAD 2 AT THE SAME TIME-----------------------------

    while (loadcount == 0) { //UNLOAD 2 AT THE SAME TIME
      while (bottlefront == LOW) {
        bottlefront = digitalRead(2);
        if (dispcheck == 0) {
          Serial.println("CONVEYOR TRANSFERRING 2 LOADS TO DEPOT");
          dispcheck = 1;
        }

        digitalWrite(6, HIGH);

        if (bottlefront == HIGH) {
          dispcheck = 0;
          break;
        }
      }

      if (transcheck == 0 && bottlefront == HIGH) {
        delay(1000); //extra time to fully unload
        digitalWrite(6, LOW);
        break;
      }
      else {
        while (bottlefront == HIGH) {
          if (dispcheck == 0) {
            dispcheck = 1;
            Serial.println("Intermission");
          }

          bottlefront = digitalRead(2);
          transcheck = 0;

          if (bottlefront == LOW) {
            dispcheck = 0;
            break;
          }
        }

      }
    }

    //------------------------UNLOAD 1 AT FRONT-----------------------------------

    if (loadcount == 1) { //UNLOAD 1 AT FRONT
      while (bottlefront == LOW) {
        bottlefront = digitalRead(2);
        if (dispcheck == 0) {
          dispcheck = 1;
          Serial.println("CONVEYOR TRANSFERRING 1 FRONT LOAD TO DEPOT");
        }

        digitalWrite(6, HIGH);

        if (bottlefront == HIGH) {
          delay(1000); //extra time to fully unload
          digitalWrite(6, LOW);
          dispcheck = 0;
          break;
        }
      }
    }


    //---------------------------------UNLOAD 1 AT BACK----------------------------------

    while (loadcount == 2) { //UNLOAD 1 AT BACK
      while (bottleback == LOW) {
        if (dispcheck == 0) {
          dispcheck = 1;
          Serial.println("CONVEYOR TRANSFERRING 1 BACK LOAD TO DEPOT, LOAD IS CURRENTLY BEHIND");
        }

        bottleback = digitalRead(3);
        digitalWrite(6, HIGH);
        transcheck = 2;

        if (bottleback == HIGH && bottlefront == LOW) {
          dispcheck = 0;
          break;
        }
      }
      bottlefront = digitalRead(2);


      while (bottlefront == LOW && transcheck == 2) {
        bottlefront = digitalRead(2);
        digitalWrite(6, HIGH);
        if (dispcheck == 0) {
          dispcheck = 1;
          Serial.print("Transfer check = ");
          Serial.print(transcheck);
          Serial.print(" bottlefront = ");
          Serial.print(bottlefront);
          Serial.println(" CONVEYOR TRANSFERRING LOAD TO DEPOT, LOAD IS CURRENTLY FRONT");
        }


        for (int i = 0; bottlefront == HIGH; i++) { //check for sensor inconsistency
          bottlefront = digitalRead(2);
          Serial.println(i);
          delay(20);
          if (i > 5) {
            transcheck = 0;
            delay(1000); //extra time to fully unload
            digitalWrite(6, LOW);
            break;
          }
        }

      }

      if (transcheck == 0) {
        break;
        dispcheck = 0;
      }
    }


    //--------------------------------------LOAD 2 ONTO ROBOT----------------------------------

    while (loadcount == 3) { //replace with docking communication
      while (bottlefront == HIGH) {
        bottlefront = digitalRead(2);
        digitalWrite(6, HIGH);
        if (dispcheck == 0) {
          dispcheck = 1;
          Serial.println("CONVEYOR TRANSFERRING 2 LOADS FROM DEPOT TO VEHICLE");
        }
        if (bottleback == LOW) {
          Serial.println(", OBJECT CURRENTLY AT MID POINT");
        }
        // replace for presentation
        if (bottlefront == LOW) {
          transcheck = 0;
          digitalWrite(6, LOW);
          break;
        }
        if (bottleback == LOW) { 
          transcheck = 0;
          digitalWrite(6, LOW);
          break;
        }
        else { 
          delay(5000);
          transcheck = 0;
          break;
        }
      }

      if (transcheck == 0) {
        dispcheck = 0;
        break;
      }
    }

    //---------------------------------------LOAD 1 ONTO ROBOT---------------------------------

    /*while (loadcount == 3) { //replace with docking communication
      while (bottleback == HIGH) {
        bottleback = digitalRead(3);
        digitalWrite(6, HIGH);
        if (dispcheck == 0) {
          dispcheck = 1;
          Serial.println("CONVEYOR TRANSFERRING ONLY 1 LOAD FROM DEPOT TO VEHICLE");
        }
        if (bottleback == LOW) {
          transcheck = 0;
          digitalWrite(6, LOW);
          break;

        }
      }
      if (transcheck == 0) {
        break;
      }
      }

      digitalWrite(6, LOW);
      delay(1000);
      Serial.println("CONVEYOR LOAD SUCCESSFULLY TRANSFERRED");*/

    //-----------------------LOWERING SEQUENCE-------------------------
    if (loadcount == 4) {
      digitalWrite(6, HIGH);
      delay(3000);
      digitalWrite(6, LOW);

    }
    while (downcheck == 0) {
      /*Serial.println("Enter 3 to begin descending to transit position process"); //manual control for lowering using serial
        if (Serial.available() > 0) {
        dockinfo = Serial.read();
        }*/
      delay(1500);
      dockinfo = 51; //preset for lowe  ring + delay
      if (dockinfo == 51) {
        while (movcheck == 0) {
          scisspb = digitalRead(7);
          UU = digitalRead(8);
          height[2] = digitalRead(11);
          height[1] = digitalRead(12);
          digitalWrite(4, HIGH);
          digitalWrite(5, HIGH);
          /*if (dispcheck == 0) {
            dispcheck = 1;
            //Serial.println("SCISSOR LIFT GOING DOWN TO TRANSIT POSITION");
            }*/

          /*if (scisspb == 1) {
            stopmov = 1;
            goto uugoto;
            break;
            }*/
          if (digitalRead(12) == HIGH || digitalRead(11) == HIGH) {
            digitalWrite(5, LOW);
            dispcheck = 0;
            dockinfo = 0;
            downcheck = 1;
            movcheck = 1;
            delay(1000);
            Serial.write(50);
            check = 0;
            entrans = 0;
            movcheck = 0;
            tempcheck = 0;
            serialcheck = 0;
            Serial.write(50);
            break;

          }
          /*for (int c = 0; height[2] == HIGH; c++) { //sensor inconsistency
            height[2] = digitalRead(11);
            Serial.println(c);
            delay(20);
            if (c > 5) {
              Serial.println(dockinfo);
              dispcheck = 0;
              dockinfo = 0;
              downcheck = 1;
              movcheck = 1;
              break;
              Serial.println(dockinfo);
            }
            }*/
          /*if (UU == LOW) {
            Serial.println("REACHED LOWER LIMIT, EXTENDING AND RESETTING");
            digitalWrite(5, LOW);
            delay(1000);
            digitalWrite(4, LOW);
            digitalWrite(5, HIGH);
            delay(1500);
            limitcheck = 1;
            uugoto:
            dispcheck = 0;
            dockinfo = 0;
            downcheck = 1;
            movcheck = 1;
            break;
            }*/
        }
        if (downcheck == 1) {
          downcheck = 0;
          break;
        }
      }
    }
    digitalWrite(5, LOW);
    //Serial.println("SCISSOR LIFT STOPPED AT BOTTOM, CAN NOW BE TRANSPORTED");

    //undock vehicle and start moving towards next destination

    /*delay(1000);
      Serial.write(50);
      check = 0;
      entrans = 0;
      movcheck = 0;
      tempcheck = 0;
      serialcheck = 0;
      Serial.write(50); //Tell the robot that it can continue on its journey*/

  }

  delay(40);

}

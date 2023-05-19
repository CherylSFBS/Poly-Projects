

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(10, INPUT); //track detection
  pinMode(11, INPUT); //MARKER LEFT
  pinMode(12, INPUT); //MARKER RIGHT
}

float stabcalc1;
float stabcalc2;
byte stopmagstrip = 0;

void loop() {
  bool LMARK = digitalRead(11); //HIGH = DETECTED, LOW = UNDETECTED
  bool RMARK = digitalRead(12); //HIGH = DETECTED, LOW = UNDETECTED
  bool trackdet = digitalRead(10);
  float track = analogRead(A0); //avg 1.75
  track = (track * 5 / 1023) - 0.17;

  /*if (track < 0.415) {
    track = 0.415; //IDK? SENSOR DOESNT EVEN DETECT PROPERLY
    }*/

  float stabiliser = track * 17.647; //at 1.7 will be 30, at 0.5 will be 8.8235, at 2.5 will be 44.1175, at 3 will be 52


  //the further the value of stabiliser from 30, the weaker fwdstaab, the stronger yawstab

  //fwdstab
  //make 44.1175 into <30 (ex. 25)
  //make 31 into as close <30 (ex. 29)
  //yawstab
  //make 8.8235 into as far >15 (ex. 19)
  //make 29 into into as close >15 (ex. 16)
  /* if (stabiliser > 30) {
     stabcalc = stabiliser - 30;
    }
    else if (stabiliser < 30) {
     stabcalc = 30 - stabiliser;
    }
    else if (stabiliser = 30) {
     stabcalc = 0;
    }


    float fwdstab = 30 + sqrt(pow(pow(stabcalc, 0.9),2));
    float yawstab = 15 + sqrt(pow(pow(stabcalc, 0.9),2));*/
  /*
    stabcalc1 = pow(30 - stabiliser, 0.9);
    stabcalc2 = pow(30 - stabiliser, 0.9);

    if (stabiliser > 30) {
      stabcalc1 = -stabcalc1;
    }
    else if (stabiliser < 30) {
      stabcalc2 = -stabcalc2;
    }
    else if (stabiliser = 30) {
      stabcalc1 = 0;
      stabcalc2 = 0;
    }


    float fwdstab = 30 + stabcalc1;
    float yawstab = 30 + stabcalc2;*/

  /*  float stabcalc = pow((30 - stabiliser), 1.2)

    if (stabiliser > 30) {
      fwdstab = 30 - stabcalc;
      yawstab = 15 + stabcalc;
    }
    else if(stabiliser < 30){
      fwdstab = 15 + stabcalc;
      yawstab = 30 - stabcalc;
    }
  */

  /*if (stabiliser < 14.08) { //setting min
    stabiliser = 14.08;
    }
    else if (stabiliser > 42.93) { //setting max
    stabiliser = 42.93;
    }*/
  // (1/800)(stabiliser - 30)^3 + 30
  /*float stabcalc1 = stabiliser * 2.5 - 75;
    float stabcalc2 = pow(stabcalc1, 3);
    float stabcalc3 = (1.00 / 800.00) * stabcalc2;
    float stabcalc = stabcalc3 + 30;


    float fwdstab = 60 - stabcalc;
    float yawstab = stabcalc;


    if (fwdstab >= 60) {
      fwdstab = 30;
      yawstab = -30;
    }

    else if (yawstab >= 60) {
      yawstab = 30;
      fwdstab = -30;
    }*/

  /*float stabcalc1 = stabiliser * 1.5 - 39;
    float stabcalc2 = pow(stabcalc1, 3);
    float stabcalc3 = (1.00 / 800.00) * stabcalc2;
    float stabcalc = stabcalc3 + 20;


    float fwdstab = 40 - stabcalc;
    float yawstab = stabcalc;*/
  float stabcalc1 = stabiliser * 1 - 26;
  float stabcalc2 = pow(stabcalc1, 3);
  float stabcalc3 = (1.00 / 100.00) * stabcalc2;
  float stabcalc = stabcalc3 + 30;

  float fwdstab = 60 - stabcalc;
  float yawstab = stabcalc;
  
  if (LMARK == HIGH) {
    fwdstab = -fwdstab;
    yawstab = -yawstab;
  }

  Serial.print(stabiliser);
  Serial.print("    ");
  Serial.print(track);

  Serial.print("    ");
  Serial.print(stabcalc2);

  Serial.print("    ");


  Serial.print(", LMARK = ");
  Serial.print(LMARK);
  Serial.print(", RMARK = ");
  Serial.print(RMARK);
  Serial.print(", Trackdet = ");
  Serial.print(trackdet);
  Serial.print(", fwdstab = ");
  Serial.print(fwdstab);
  Serial.print(", yawstab = ");
  Serial.println(yawstab);



  int fintransfer;
  if (Serial.available() > 0) {
    fintransfer = Serial.read();
  }
  if (RMARK == HIGH && stopmagstrip == 0) { //RIGHT MARKER = STOP FOR DOCKING
    stopmagstrip = 1;
    Serial3.write(1); //communicate with the mounted arduino to start scissor lift moving process
  }
  else if (fintransfer == 1 && stopmagstrip == 1) {
    stopmagstrip = 2;
  }
  else if (RMARK == LOW && stopmagstrip == 2) {
    stopmagstrip = 0;
  }
}

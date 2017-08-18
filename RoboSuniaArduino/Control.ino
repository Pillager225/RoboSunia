int dirPins[] = {7, 8};
int pwmPins[] = {10, 11};
char curDirs[] = {0, 0};
char curPWMs[] = {0, 0};

double desiredAngularSpeed[] = {0, 0};
double maxAngularSpeed = 100;

unsigned long lasttime = 0;

void handleSerialInput() {
  int len = getInput(input);
  if(len > 0) {
    if(len == 4) {
      // interpet the input as commands for the motors
      char dirs[2], pwms[2];
      memcpy(dirs, &input[0], 2);
      memcpy(pwms, &input[2], 2);
      setDirections(dirs);
      setPWMs(pwms);
      lasttime = millis();
    } else if(strcmp(input, "reset") == 0) {
      waitForSerialConnection();
    }
  } else {
    if(lasttime > millis()) {
      // millis() overflowed because the arduino has been running for more than 50 days
      lasttime = millis();
    } else if(millis()-lasttime >= 500) {
      // haven't recieved communication from main processor (Cherry Trail) in half of a second
      // this might be caused by some network error so
      // stop to robot from moving so that it doesn't crash
      char d[] = {0,0};
      setPWMs(d);
    }
  }
}

void setupControlPins() {
  DDRE |= 01000000; // setup pin 7 as output
  DDRB |= 11010000; // setup pin 11, 10, and 8 as output
//  for(int i = 0; i < 2; i++) {
//    pinMode(dirPins[i], OUTPUT);
//    pinMode(pwmPins[i], OUTPUT);
//  } 
}

// dirs should have two indicies
// dirs[i] should correspond to dirPins[i]
void setDirections(char *dirs) {
  
  for(int i = 0; i < 2; i++) {
    if(dirs[i] != curDirs[i]) {
      curDirs[i] = dirs[i];
      if(i == 0) {
        if(dirs[i]) 
          PORTE |= 01000000;
        else 
          PORTE &= 10111111;
      } else {
        if(dirs[i])
          PORTB |= 00010000;
        else
          PORTB &= 11101111;
      }
      //digitalWrite(dirPins[i], dirs[i]);
    }
  }
}

// pwms should have two indicies
// pwms[i] should correspond to pwmPins[i]
// pwms is a char * because analogWrite() only accepts pwm values
// valuse from 0-255 and there is no reason to waste extra memory
// by using ints
void setPWMs(char *pwms) {
  for(int i = 0; i < 2; i++) {
    if(pwms[i] != curPWMs[i]) {
      curPWMs[i] = pwms[i];
      analogWrite(pwmPins[i], pwms[i]);
      desiredAngularSpeed[i] = maxAngularSpeed*pwms[i]/255.0;
    }
  }
}

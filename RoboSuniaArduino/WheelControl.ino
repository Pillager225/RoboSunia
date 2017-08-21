int dirPins[] = {7, 8};
int pwmPins[] = {3, 5};
char curDirs[] = {0, 0};
char curPWMs[] = {0, 0};

double desiredAngularSpeed[] = {0, 0};
double maxAngularSpeed = 100;

void setupControlPins() {
  for(int i = 0; i < NUM_WHEELS; i++) {
    pinMode(dirPins[i], OUTPUT);
    pinMode(pwmPins[i], OUTPUT);
  } 
}

void stopWheels() {
  char d[] = {0,0};
  setPWMs(d);
}

// dirs should have two indicies
// dirs[i] should correspond to dirPins[i]
void setDirections(char *dirs) {
  for(int i = 0; i < NUM_WHEELS; i++) {
    if(dirs[i] != curDirs[i]) {
      curDirs[i] = dirs[i];
      digitalWrite(dirPins[i], dirs[i]);
    }
  }
}

// pwms should have two indicies
// pwms[i] should correspond to pwmPins[i]
// pwms is a char * because analogWrite() only accepts pwm values
// values from 0-255 and there is no reason to waste extra memory
// by using ints
void setPWMs(char *pwms) {
  for(int i = 0; i < NUM_WHEELS; i++) {
    if(pwms[i] != curPWMs[i]) {
      curPWMs[i] = pwms[i];
      analogWrite(pwmPins[i], pwms[i]);
      desiredAngularSpeed[i] = maxAngularSpeed*pwms[i]/255.0;
    }
  }
}

void addToPWM(int val, int motor) {
  curPWMs[motor] += val;
  curPWMs[motor] = curPWMs[motor] > 255 ? 255 : curPWMs[motor];
  curPWMs[motor] = curPWMs[motor] < 0 ? 0 : curPWMs[motor];
  analogWrite(pwmPins[motor], curPWMs[motor]);
}


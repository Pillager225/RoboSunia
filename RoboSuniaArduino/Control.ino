int dirPins[] = {7, 8};
int pwmPins[] = {10, 11};
char curDirs[] = {0, 0};
char curPWMs[] = {0, 0};

void setupControlPins() {
  for(int i = 0; i < 2; i++) {
    pinMode(dirPins[i], OUTPUT);
    pinMode(pwmPins[i], OUTPUT);
  } 
}

// dirs should have two indicies
// dirs[i] should correspond to dirPins[i]
void setDirections(char *dirs) {
  for(int i = 0; i < 2; i++) {
    if(dirs[i] != curDirs[i]) {
      curDirs[i] = dirs[i];
      digitalWrite(dirPins[i], dirs[i]);
    }
  }
}

// pwms should have two indcies
// pwms[i] should correspond to pwmPins[i]
// pwms is a char * because analogWrite() only accepts pwm values
// valuse from 0-255 and there is no reason to waste extra memory
// by using ints
void setPWMs(char *pwms) {
  for(int i = 0; i < 2; i++) {
    if(pwms[i] != curPWMs[i]) {
      curPWMs[i] = pwms[i];
      analogWrite(pwmPins[i], pwms[i]);
    }
  }
}

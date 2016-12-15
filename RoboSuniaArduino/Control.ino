int dirPins[] = {7, 8};
int pwmPins[] = {10, 11};

void setupControlPins() {
  for(int i = 0; i < 2; i++) {
    pinMode(dirPins[i], OUTPUT);
    pinMode(pwmPins[i], OUTPUT);
  } 
}

// dirs should have two indicies
// dirs[i] should correspond to dirPins[i]
void setDirections(char *dirs) {
  digitalWrite(dirPins[0], dirs[0]);
  digitalWrite(dirPins[1], dirs[1]);
}

// pwms should have two indcies
// pwms[i] should correspond to pwmPins[i]
// pwms is a char * because analogWrite() only accepts pwm values
// valuse from 0-255 and there is no reason to waste extra memory
// by using ints
void setPWMs(char *pwms) {
  analogWrite(pwmPins[0], pwms[0]);
  analogWrite(pwmPins[1], pwms[1]);
}

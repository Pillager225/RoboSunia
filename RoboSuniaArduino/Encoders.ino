int encoderPins[] = {4, 5};
int blipsPerRotation = 40;
const int sampleSize = 10;
long lastBlipTime[2];
long blipDiff[2][sampleSize];
int blipIndex[2];
int blipCount[2];
const int dWeight = 5;

void encoderISR(int i) {
  long now = millis();
  blipDiff[i][blipIndex[i]] = now-lastBlipTime[i];
  lastBlipTime[i] = now;
  blipIndex[i] = blipIndex[i]+1 == sampleSize ? 0 : blipIndex[i]+1;
  blipCount[i]++;
}

void lEncoderISR() {
  encoderISR(0);
}

void rEncoderISR() {
  encoderISR(1);
}

double getAngularSpeed(int motor) {
  long blipDiffAve = 0;
  for(int i = 0; i < sampleSize; i++) {
    blipDiffAve += blipDiff[motor][i];
  }
  blipDiffAve /= sampleSize;
  // 5 inch wheels in diameter/4 because there are 40 state changes caused by the encoder in one rotation
  // (2*pi*r/(blipsPerRotation/sampleSize))/blipDiffAve
  return (31.4159265358979*sampleSize)/(blipsPerRotation*blipDiffAve);
}

void setupEncoders() {
  for(int i = 0; i < 2; i++) {
    pinMode(encoderPins[i], INPUT);
  }
  attachInterrupt(digitalPinToInterrupt(encoderPins[0]), lEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPins[1]), rEncoderISR, CHANGE);
}

void controlPWMs() {
  for(int i = 0; i < 2; i++) {
    if(desiredAngularSpeed[i] >= .5) {
      curPWMs[i] = 0;
      analogWrite(pwmPins[i], curPWMs[i]);
    } else {
      double speedDiff = getAngularSpeed(i)-desiredAngularSpeed[i];
      if(doubleAbs(speedDiff) >= 2) {
        curPWMs[i] += speedDiff*dWeight;
        analogWrite(pwmPins[i], curPWMs[i]);
      }
    }
  }
}


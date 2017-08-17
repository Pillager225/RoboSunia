// inches
#define WHEEL_DIAMETER 5
#define BLIPS_PER_ROTATION 40
#define SAMPLE_SIZE 10
#define VEL_DEADZONE .1
#define VEL_ERROR_THRESHOLD 2

const int dWeight = 5, encoderPins[] = {4, 5};
double desiredAngularSpeed[] = {0, 0};
Encoder encoders[2];

void setupEncoders() {
  encoders[LEFT] = Encoder(encoderPins[LEFT], BLIPS_PER_ROTATION, SAMPLE_SIZE, WHEEL_DIAMETER);
  encoders[RIGHT] = Encoder(encoderPins[RIGHT], BLIPS_PER_ROTATION, SAMPLE_SIZE, WHEEL_DIAMETER);
}

void controlPWMs() {
  for(int i = 0; i < 2; i++) {
    if(desiredAngularSpeed[i] <= VEL_DEADZONE) {
      curPWMs[i] = 0;
      analogWrite(pwmPins[i], curPWMs[i]);
    } else {
      double speedDiff = desiredAngularSpeed[i]-encoders[i].getSpeed();
      if(doubleAbs(speedDiff) >= VEL_ERROR_THRESHOLD) {
        curPWMs[i] += speedDiff*dWeight;
        analogWrite(pwmPins[i], curPWMs[i]);
      }
    }
  }
}


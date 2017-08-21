const int dWeight = 5, encoderPins[] = {4, 5};
Encoder encoders[2];

void isrLeft() {
  encoders[LEFT].isr();
}

void isrRight() {
  encoders[RIGHT].isr();
}

void setupEncoders() {
  encoders[LEFT] = Encoder(encoderPins[LEFT], BLIPS_PER_ROTATION, SAMPLE_SIZE, WHEEL_DIAMETER);
  encoders[RIGHT] = Encoder(encoderPins[RIGHT], BLIPS_PER_ROTATION, SAMPLE_SIZE, WHEEL_DIAMETER);
  attachInterrupt(digitalPinToInterrupt(encoderPins[LEFT]), isrLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPins[RIGHT]), isrRight, CHANGE);
}

void controlPWMs() {
  for(int i = 0; i < 2; i++) {
    addToPWM(encoders[i].getSpeedControlEffort(), i);
  }
}


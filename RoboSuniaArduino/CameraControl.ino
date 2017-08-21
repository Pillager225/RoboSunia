int servoPins[] = {9, 10};

Servo servos[2];

void setupCameraServos() {
  for(int i = 0; i < 2; i++) {
    servos.attach(servoPins[i]);
    servos.write(90);
  }
}


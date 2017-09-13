int servoPins[] = {9, 10}, servoAngles[] = {30, 90};
Servo servos[NUM_CAMERA_SERVOS];

void setupCameraServos() {
  for(int i = 0; i < NUM_CAMERA_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(servoAngles[i]);
  }
}

// servoCommands should have two indicies
void moveCameraServos(char *servoCommands) {
  for(int i = 0; i < NUM_CAMERA_SERVOS; i++) {
    // 2 means positive movement
    if(servoCommands[i] == '2') {
      servoAngles[i] += MOVING_SPEED;
      servos[i].write(servoAngles[i]);
    } else if(servoCommands[i] == '0') {
      servoAngles[i] -= MOVING_SPEED;
      servos[i].write(servoAngles[i]);
    }
  }
}


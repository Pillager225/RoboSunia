int VERTICLE = 0, HORIZONTAL = 1;
int servoPins[] = {9, 10}, servoAngles[] = {45, 90}, angLowLimit[] = {25, 10}, angHighLimit[] = {80, 170};
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
      servoAngles[i] = servoAngles[i] <= angHighLimit[i] ? servoAngles[i] : angHighLimit[i];
      servos[i].write(servoAngles[i]);
    } else if(servoCommands[i] == '0') {
      servoAngles[i] -= MOVING_SPEED;
      servoAngles[i] = servoAngles[i] >= angLowLimit[i] ? servoAngles[i] : angLowLimit[i];
      servos[i].write(servoAngles[i]);
    } else if(servoCommands[i] == '3') {
      servoAngles[i] = servoAngles[i];
      servos[i].write(servoAngles[i]);
    }
  }
}


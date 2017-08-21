#include <Encoder.h>
#include <DistanceSensor.h>
#include <Servo.h>

#define MAX_BUFF_SIZE 64
#define LEFT 0
#define RIGHT 1

char input[MAX_BUFF_SIZE];

void setup() {
  Serial.begin(9600);
  setupControlPins();
  setupDistanceSensor();
  setupEncoders();
  setupCameraServos();
  waitForSerialConnection(); 
}

void loop() {
  handleSerialInput();
  handleDistanceSensors();
  controlPWMs();
}

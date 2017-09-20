#include <Encoder.h>
#include <DistanceSensor.h>
#include <Servo.h>

#define MAX_BUFF_SIZE 64
#define LEFT 0
#define RIGHT 1

// WheelControl
#define NUM_WHEELS 2 

// Encoders
// inches
#define WHEEL_DIAMETER 5
#define BLIPS_PER_ROTATION 40
#define SAMPLE_SIZE 10

// DistanceSensor
#define NUM_SENSORS 1
#define FRONT_SENSOR 0
// in milliseconds
#define SENSOR_READING_PERIOD 300
#define MESSAGE_LENGTH 16
#define NUM_VALUES 11

// COMM
// in milliseconds
#define COMM_TIMEOUT 500
#define COMMAND_PACKET_LENGTH 6

// CameraControl
// in degrees
#define MOVING_SPEED 3
#define NUM_CAMERA_SERVOS 2

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

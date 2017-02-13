#define MAX_BUFF_SIZE 64

char input[MAX_BUFF_SIZE];

void setup() {
  Serial.begin(9600);
  setupControlPins();
  setupDistanceSensor();
  setupEncoders();
  waitForSerialConnection(); 
}

void loop() {
  handleSerialInput();
  handleDistSensors();
}

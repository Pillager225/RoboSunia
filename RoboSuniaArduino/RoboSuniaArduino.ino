#define MAX_BUFF_SIZE 64

char input[MAX_BUFF_SIZE];

void setup() {
  Serial.begin(9600);
  setupControlPins();
  waitForSerialConnection(); 
}

void loop() {
  int len = waitForInput(input);
  if(len > 0) {
    if(len == 4) {
      // interpet the input as commands for the motors
      char dirs[2], pwms[2];
      memcpy(dirs, &input[0], 2);
      memcpy(pwms, &input[2], 2);
      setDirections(dirs);
      setPWMs(pwms);
    } else if(strcmp(input, "reset") == 0) {
      waitForSerialConnection();
    }
  }
}

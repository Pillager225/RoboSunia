#define MAX_BUFF_SIZE 64

char input[MAX_BUFF_SIZE];
unsigned long lasttime = 0;

void setup() {
  Serial.begin(9600);
  setupControlPins();
  waitForSerialConnection(); 
}

void loop() {
  int len = getInput(input);
  if(len > 0) {
    if(len == 4) {
      // interpet the input as commands for the motors
      lasttime = millis();
      char dirs[2], pwms[2];
      memcpy(dirs, &input[0], 2);
      memcpy(pwms, &input[2], 2);
      setDirections(dirs);
      setPWMs(pwms);
    } else if(strcmp(input, "reset") == 0) {
      waitForSerialConnection();
    }
  } else {
    if(lasttime > millis()) {
      // millis() overflowed because the arduino has been running for more than 50 days
      lasttime = millis();
    } else if(millis()-lasttime >= 500) {
      // haven't recieved communication from main processor (Cherry Trail) in half of a second
      // this might be caused by some network error so
      // stop to robot from moving so that it doesn't crash
      char d[] = {0,0};
      setPWMs(d);
    }
  }
}

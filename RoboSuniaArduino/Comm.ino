// The distance sensor communication in handled in Distance Sensor, mostly because this file is so long already
unsigned long lasttime = 0;
char input[MAX_BUFF_SIZE];

int getInput(char *buff) {
   int avail = Serial.available();
   if(avail <= 0) {
     return 0;
   } else {
     for(int i = 0; i < avail && i < MAX_BUFF_SIZE; i++) {
       buff[i] = Serial.read();
     }
     if(avail < MAX_BUFF_SIZE) {
       buff[avail] = '\0';  
       return avail;
     } else {
       buff[MAX_BUFF_SIZE-1] = '\0';
       return MAX_BUFF_SIZE;
     }
   }
}

void waitForSerialConnection() {
  stopWheels();
  while(!Serial);
  while(true) {
    delay(1000);
    Serial.println("Arduino");
    char input[MAX_BUFF_SIZE];
    int len = getInput(input);
    if(len > 0) {
      if(strcmp("connected", input) == 0) {
        return; 
      }
    }
  }
}

void reactToSerialData() {
  setDirections(&input[0]);
  setPWMs(&input[NUM_WHEELS]);
  moveCameraServos(&input[NUM_WHEELS+NUM_WHEELS]);
}

void handleSerialInput() {
  int len = getInput(input);
  if(len > 0) {
    if(len == COMMAND_PACKET_LENGTH) {
      reactToSerialData();
      lasttime = millis();
    } else if(strcmp(input, "reset") == 0) {
      waitForSerialConnection();
    }
  } else {
    if(lasttime > millis()) {
      // millis() overflowed because the arduino has been running for more than 50 days
      lasttime = millis();
    } else if(millis()-lasttime >= COMM_TIMEOUT) {
      // haven't recieved communication from main processor (Cherry Trail) in half of a second
      // this might be caused by some network error so
      // stop to robot from moving so that it doesn't crash
      stopWheels();
    }
  }
}

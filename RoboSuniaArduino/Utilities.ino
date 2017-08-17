int getInput(char *buff) {
   int avail = Serial.available();
   if(avail <= 0) {
     return 0;
   } else {
     for(int i = 0; i < avail; i++) {
       buff[i] = Serial.read();
     }
     buff[avail] = '\0'; 
     return avail;
   }
}

void waitForSerialConnection() {
  // stop the motors if they are going
  char d[] = {0,0};
  setPWMs(d);
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

double doubleAbs(double d) {
  if(d < 0) {
    return -d;
  } else {
    return d;
  }
}


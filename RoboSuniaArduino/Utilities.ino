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

//int waitForInput(char *buff) {
//   int avail;
//   while((avail = Serial.available()) <= 0);  
//   for(int i = 0; i < avail; i++) {
//     buff[i] = Serial.read();
//   }
//   buff[avail] = '\0'; 
//   return avail;
//}

void waitForSerialConnection() {
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

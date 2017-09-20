DistanceSensor dSenses[NUM_SENSORS];
int inputPins[] = {A5};
long lasttimeDist = 0;


void setupDistanceSensor() {
  dSenses[FRONT_SENSOR] = DistanceSensor(inputPins[FRONT_SENSOR]);
}

String padOutput(String o, int dLen) {
  if(o.length() > dLen) {
    return o;
  }
  int padding = dLen-o.length();
  for(int i = 0; i < padding; i++) {
    o += ' ';
  }
  o += '\r';
  return o;
}

void handleDistanceSensors() {
  if(millis()-lasttimeDist >= SENSOR_READING_PERIOD) {
    dSenses[FRONT_SENSOR].getDistanceReading();  
    Serial.print(padOutput(String(dSenses[FRONT_SENSOR].getDistanceReading()), MESSAGE_LENGTH));
    lasttimeDist = millis();
  } else if(lasttime > millis()) {
    // millis() overflowed because the arduino has been running for more than 50 days
    lasttime = millis();
  }
}

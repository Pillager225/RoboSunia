#define NUM_SENSORS 1
#define FRONT_SENSOR 0
// in milliseconds
#define SENSOR_READING_PERIOD 300
#define MESSAGE_LENGTH 32
#define NUM_VALUES 11

DistanceSensor dSenses[NUM_SENSORS];
int inputPins[] = {A5};
int voltVals[NUM_VALUES] = {603, 452, 264, 188, 150, 127, 108, 96, 85, 77, 70}; // analogRead vals
int dists[NUM_VALUES] = {2, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31}; // inches
long lasttimeDist = 0;


void setupDistanceSensor() {
  dSenses[FRONT_SENSOR] = DistanceSensor(inputPins[FRONT_SENSOR], NUM_VALUES, &voltVals[0], &dists[0]);
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
    Serial.print(padOutput(dSenses[FRONT_SENSOR].interperateDistanceReadings(), MESSAGE_LENGTH));
    lasttimeDist = millis();
  } else if(lasttime > millis()) {
    // millis() overflowed because the arduino has been running for more than 50 days
    lasttime = millis();
  }
}

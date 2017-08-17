const int FRONT_SENSOR = 0;

const int numSensors = 1; // number of distance sensors
DistanceSensor dSenses[numSensors];
int inputPins[] = {A5};
const int numOfValues = 11;
int voltVals[numOfValues] = {603, 452, 264, 188, 150, 127, 108, 96, 85, 77, 70}; // analogRead vals
int dists[numOfValues] = {2, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31}; // inches
long lasttimeDist = 0;


void setupDistanceSensor() {
  dSenses[FRONT_SENSOR] = DistanceSensor(inputPins, numOfValues, voltVals, dists);
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
  if(millis()-lasttimeDist >= 300) {
    dSenses[FRONT_SENSOR].getDistanceReading();  
    Serial.print(padOutput(dSenses[FRONT_SENSOR].interperateDistanceReadings(), 32));
    lasttimeDist = millis();
  } else if(lasttime > millis()) {
    // millis() overflowed because the arduino has been running for more than 50 days
    lasttime = millis();
  }
}

const int FRONT_SENSOR = 0;

int inputPins[] = {A5};
const int numInputPins = 1; // the length of the inputPin array
             // 3.2,  1, .5.
const int numOfValues = 11;
int voltVals[numOfValues] = {603, 452, 264, 188, 150, 127, 108, 96, 85, 77, 70}; // analogRead vals
int dists[numOfValues] = {2, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31}; // inches

const int numOfReadings = 10;
int distanceReadings[numInputPins][numOfReadings];
int readingIndex[numInputPins];
int valSampleSize = 10;

void setupDistanceSensor() {
  for(int i = 0; i < numInputPins; i++) {
    pinMode(inputPins[i], INPUT);
  }
}

int findDistanceFromAnalogVal(int val) {
  int dist = 0;
  if( val >= voltVals[0]) {
    Serial.println("to close or far");
    return 0;
  }
  int i;
  for(i = 1; i < numOfValues; i++) {
    if(val == voltVals[i-1]) {
      return dists[i-1];
    }  else if(val == voltVals[i]) {
      return dists[i];
    } else if(val <= voltVals[i-1] && val >= voltVals[i]) {
      // linearly interpolate to find distance
      return dists[i-1]+(val - voltVals[i-1])*(dists[i]-dists[i-1])/(voltVals[i]-voltVals[i-1]);
    }
  }
  if(val < voltVals[i]) {
      return 32;
  } 
  return dist;
}

int getReading(int pin) {
  long valAve = 0;
  for(int i = 0; i < valSampleSize; i++) {
    valAve += analogRead(pin);
  }
  return valAve/valSampleSize;
}

int getCurrentDistance(int inputPinsIndex) { 
  int analogVal = getReading(inputPins[inputPinsIndex]);
 // Serial.println(analogVal);
  return findDistanceFromAnalogVal(analogVal);
}

String interpertDistanceReadings(int sensor) {
  int prevIndex = readingIndex-1 < 0 ? numOfReadings-1 : readingIndex[sensor]-1;
  int prevprevIndex = prevIndex-1 < 0 ? numOfReadings-1 : prevIndex-1;
  if((distanceReadings[sensor][prevprevIndex] > 2 && distanceReadings[sensor][prevprevIndex] < 7)
      && distanceReadings[sensor][prevIndex] < distanceReadings[sensor][prevprevIndex] 
      && distanceReadings[sensor][readingIndex[sensor]] > distanceReadings[sensor][prevprevIndex]+2) {
    return String("Possibly too close");
  }
  if(distanceReadings[sensor][readingIndex[sensor]] >= 31) {
    return String("31+ inches");
  }
  return String(distanceReadings[sensor][readingIndex[sensor]])+ " inches";
}

void handleDistSensors() {
  int curDist = getCurrentDistance(FRONT_SENSOR);  
  distanceReadings[FRONT_SENSOR][readingIndex[FRONT_SENSOR]] = curDist;
  readingIndex[FRONT_SENSOR] = readingIndex[FRONT_SENSOR]+1 == numOfReadings ? 0 : readingIndex[FRONT_SENSOR]+1;
  Serial.println(interpertDistanceReadings(FRONT_SENSOR));
}


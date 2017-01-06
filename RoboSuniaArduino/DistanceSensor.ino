const int FRONT_SENSOR = 0;

int inputPins[] = {A5};
const int numInputPins = 1; // the length of the inputPin array
             // 3.2,  1, .5.
const int numOfValues = 5;
int voltVals[numOfValues] = {1000, 992, 310, 155, 93};
int dists[numOfValues] = {0, 5, 20, 60, 80};

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
    return 0;
  }
  int i;
  for(i = 1; i < numOfValues; i++) {
    if(val == voltVals[i-1]) {
      return voltVals[i-1];
    }  else if(val == voltVals[i]) {
      return voltVals[i];
    } else if(val >= voltVals[i-1] && val <= voltVals[i]) {
      // linearly interpolate to find distance
      return dists[i-1]+(val - voltVals[i-1])*(dists[i]-dists[i-1])/(voltVals[i]-voltVals[i-1]);
    }
  }
  if(val < voltVals[i]) {
      return 90;
  } 
  return dist;
}

int getReading(int pin) {
  int valAve = 0;
  for(int i = 0; i < valSampleSize; i++) {
    valAve += analogRead(pin);
  }
  return valAve/valSampleSize;
}

int getCurrentDistance(int inputPinsIndex) { 
  int analogVal = getReading(inputPins[inputPinsIndex]);
  int distSum = 0;
  for(int i = 0; i < numOfReadings; i++) {
    distSum += findDistanceFromAnalogVal(analogVal);
  }
  return distSum/numOfReadings;
}

void handleDistSensors() {
  int curDist = getCurrentDistance(FRONT_SENSOR);  
  distanceReadings[FRONT_SENSOR][readingIndex[FRONT_SENSOR]++] = curDist;
  readingIndex[FRONT_SENSOR] = readingIndex[FRONT_SENSOR] == numOfReadings ? 0 : readingIndex[FRONT_SENSOR];
  // TODO report back to user
  Serial.write(curDist);
}


#include "DistanceSensor.h"

#define DEFAULT_SIZE 10

int DistanceSensor::findDistanceFromAnalogVal(const int &val) const {
	if(!valid) 
		return 0;
	int dist = 0;
	if(val >= voltVals[0]) {
		return -1;
	}
	for(int i = 1; i < numOfValues; i++) {
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
		return dists[numOfValues-1]+1;
	} 
	return dist;
}

DistanceSensor::DistanceSensor() {
	pin = 0;
	sampleSize = 0;
	numOfValues = 0;
	voltVals = 0;
	dists = 0;
	readingIndex = 0;
	numOfReadings = 0;
	distanceReadings = 0;
	valid = false;
}

DistanceSensor::DistanceSensor(const int &pin, const int &numOfValues, int *voltVals, int *dists, const int &numOfReadings=DEFAULT_SIZE, const int &sampleSize=DEFAULT_SIZE) {
	this->pin = pin;
	this->sampleSize = sampleSize;
	this->numOfValues = numOfValues;
	this->voltVals = voltVals;
	this->dists = dists;
	readingIndex = 0;
	this->numOfReadings = numOfReadings;
	distanceReadings = new int[numOfReadings];
	valid = true;
	pinMode(pin, INPUT);
}

DistanceSensor::~DistanceSensor() {
	delete[] distanceReadings;
}

int DistanceSensor::getDistanceReading() {
	if(!valid) 
		return 0;
	long valAve = 0;
	for(int i = 0; i < sampleSize; i++) {
		valAve += analogRead(pin);
	}
	valAve /= sampleSize;

	int dist = findDistanceFromAnalogVal(valAve);
	if(dist != -1) {
		distanceReadings[readingIndex] = dist;
		readingIndex = readingIndex+1 == numOfReadings ? 0 : readingIndex+1;
	}
	return dist;
}

String DistanceSensor::interperateDistanceReadings() const {
	int prevIndex = readingIndex-1 < 0 ? numOfReadings-1 : readingIndex[sensor]-1;
	int prevprevIndex = prevIndex-1 < 0 ? numOfReadings-1 : prevIndex-1;
	if((distanceReadings[prevprevIndex] > 2 && distanceReadings[prevprevIndex] < 7)
	  && distanceReadings[prevIndex] < distanceReadings[prevprevIndex] 
	  && distanceReadings[readingIndex] > distanceReadings[prevprevIndex]+2) {
		return String("Possibly too close");
	}
	if(distanceReadings[sensor][readingIndex[sensor]] == 0) {
		return String("31+ inches");
	}
	return String(distanceReadings[sensor][readingIndex[sensor]])+ " inches";
}
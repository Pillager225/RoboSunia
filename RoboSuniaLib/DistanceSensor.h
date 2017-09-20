#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#define DEFAULT_SIZE 10

#include <Arduino.h>

#define NUM_VALUES 11

class DistanceSensor {
protected:
	int voltVals[NUM_VALUES] = {603, 452, 264, 188, 150, 127, 108, 96, 85, 77, 70}; // analogRead vals
	int dists[NUM_VALUES] = {2, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31}; // inches
	bool valid;
	int pin, sampleSize;
	int numOfValues;
	int readingIndex, numOfReadings, *distanceReadings;

	int findDistanceFromAnalogVal(const int &val) const;
public:
	DistanceSensor();
	DistanceSensor(const int &pin, const int &numOfReadings=DEFAULT_SIZE, const int &sampleSize=DEFAULT_SIZE);
	~DistanceSensor();
	int getDistanceReading();
	String interperateDistanceReadings() const;
};

#endif
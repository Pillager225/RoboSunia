#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#define DEFAULT_SIZE 10

#include <Arduino.h>

class DistanceSensor {
protected:
	bool valid;
	int pin, sampleSize;
	int numOfValues, *voltVals, *dists;
	int readingIndex, numOfReadings, *distanceReadings;

	int findDistanceFromAnalogVal(const int &val) const;
public:
	DistanceSensor();
	DistanceSensor(const int &pin, const int &numOfValues, int *voltVals, int *dists, const int &numOfReadings=DEFAULT_SIZE, const int &sampleSize=DEFAULT_SIZE);
	~DistanceSensor();
	int getDistanceReading();
	String interperateDistanceReadings() const;
};

#endif
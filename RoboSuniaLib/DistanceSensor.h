#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <Arduino.h>

class DistanceSensor {
protected:
	int pin, sampleSize;
	int numOfValues, *voltVals, *dists;
	int readingIndex, numOfReadings, *distanceReadings;

	int findDistanceFromAnalogVal(const int &val) const;
public:
	DistanceSensor();
	DistanceSensor(const int &pin, const int &sampleSize, int *voltVals, int *dists, const int numOfReadings);
	~DistanceSensor();
	int getDistanceReading();
	String interperateDistanceReadings() const;
};

#endif
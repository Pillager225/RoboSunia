#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"

class Encoder {
protected:
	int pin, blipsPerRotation = 40, aveSize = 10;
	double wheelCircumference, coeff;
	volatile int blipIndex = 0;
	volatile long blipDiffs[aveSize], lastBlipTime = 0;
	volatile bool invalid = true;

	void isr();
	double blipsToSpeedCoeff(const int &averageLength) const;
public:
	Encoder(const int &pin, const int &blipsPerRotation, const int &sizeOfAverageWindow, const double &wheelDiameter);
	double getSpeed() const;
	bool isValid() const;
	void clear();
};

#endif
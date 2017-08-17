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
public:
	Encoder(int pin, int blipsPerRotation);
	double getSpeed();
	bool isValid();
	void clear();
};

#endif
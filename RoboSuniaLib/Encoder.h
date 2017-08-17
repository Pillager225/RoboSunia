#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
protected:
	int pin, blipsPerRotation, aveSize;
	double wheelCircumference, coeff;
	volatile int blipIndex;
	volatile long *blipDiffs, lastBlipTime;
	volatile bool invalid = true;

	void setupEncoderISRInstances();
	double blipsToSpeedCoeff(const int &averageLength) const;
public:
	Encoder();
	Encoder(const int &pin, const int &blipsPerRotation, const int &sizeOfAverageWindow, const double &wheelDiameter);
	~Encoder();
	double getSpeed() const;
	bool isValid() const;
	void clearBuff();
	void isr();
};

#endif

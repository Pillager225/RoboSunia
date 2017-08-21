#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#define VEL_DEADZONE .1
#define VEL_ERROR_THRESHOLD 2
// for proportional control of the motors
#define Kp 5

class Encoder {
protected:
	int pin, blipsPerRotation, aveSize;
	double wheelCircumference, coeff, desiredSpeed;
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
	void setDesiredSpeed(const double &speed);
	int getSpeedControlEffort();
};

#endif

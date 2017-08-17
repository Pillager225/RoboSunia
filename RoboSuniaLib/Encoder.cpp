#include <Arduino.h>
#include <stdlib.h>
#include "Encoder.h"

double Encoder::blipsToSpeedCoeff(const int &averageLength) const {
	// (wheelCircumference*(aveLen/blipsPerRotation))/(ave/aveLen)
	return wheelCircumference*averageLength*averageLength/blipsPerRotation;
}

Encoder::Encoder() {
	pin = 0;
	blipsPerRotation = 0;
	aveSize = 0;
	blipDiffs = NULL;
	wheelCircumference = 0;
	coeff = 0;
	lastBlipTime = 0;
	blipIndex = 0;
}

Encoder::Encoder(const int &pin, const int &blipsPerRotation, const int &sizeOfAverageWindow, const double &wheelDiameter) {
	this->pin = pin;
	this->blipsPerRotation = blipsPerRotation;
	aveSize = sizeOfAverageWindow;
	blipDiffs = new long[aveSize];
	wheelCircumference = wheelDiameter*PI;
	coeff = blipsToSpeedCoeff(aveSize);
	lastBlipTime = 0;
	blipIndex = 0;
	pinMode(pin, INPUT);
	memset((void *)blipDiffs, 0, aveSize*sizeof(long));
	// for(int i = 0; i < aveSize; i++) {
	// 	blipDiffs[i] = 0;
	// }
}

Encoder::~Encoder() {
	delete[] blipDiffs;
}

double Encoder::getSpeed() const {
	double ave = 0, speed = 0;
	// one should probably check if the speed is valid before asking for it
	// but if one doesn't this slower speed calculation is done
	// probably is inaccurate if accelerating
	if(invalid) {
		for(int i = blipIndex; i >= 0; i--) {
			ave += blipDiffs[i];
		}
		speed = blipsToSpeedCoeff(blipIndex+1)/ave;
	} else {
		for(int i = 0; i < aveSize; i++) {
			ave += blipDiffs[i];
		}
		speed = coeff/ave;
	}
	return speed;
}

bool Encoder::isValid() const {
	return !invalid;
}

// should be called whenever stopping, starting or switching directions
void Encoder::clearBuff() {
	blipIndex = 0;
	invalid = true;
}

void Encoder::isr() {
	long now = millis();
	blipDiffs[blipIndex] = now-lastBlipTime;
	lastBlipTime = now;
	blipIndex++;
	if(blipIndex == aveSize) {
		blipIndex = 0;
		invalid = false;
	}
}
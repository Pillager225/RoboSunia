#include "Arduino.h"
#include "Encoder.h"

#define PI 3.14159265359

Encoder::Encoder(int pin, int blipsPerRotation, int sizeOfAverageWindow double wheelDiameter) {
	pin = pin;
	blipsPerRotation = blipsPerRotation;
	aveSize = sizeOfAverageWindow;
	wheelCircumference = wheelDiameter*PI;
	coeff = blipsToSpeedCoeff(aveSize);
	pinMode(pin, INPUT);
	attachInterrupt(digitalPinToInterrupt(pin), isr, CHANGE);
	for(int i = 0; i < aveSize; i++) {
		blipDiffs[i] = 0;
	}
}

double Encoder::getSpeed() {
	double ave = 0, speed = 0;
	// one should probably check if the speed is valid before asking for it
	// but if one doesn't this slower speed calculation is done
	// probably is inaccurate if accelerating
	if(invalids) {
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

bool Encoder::isValid() {
	return !invalid;
}

// should be called whenever stopping, starting or switching directions
float Encoder::clear() {
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
		invalids = false;
	}
}

double blipsToSpeedCoeff(int averageLength) {
	// (wheelCircumference*(aveLen/blipsPerRotation))/(ave/aveLen)
	return wheelCircumference*averageLength*averageLength/blipsPerRotation;
}
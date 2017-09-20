// RoboSuniaDataTransmission.cpp : Defines the entry point for the console application.
#include "SocketServer.h"
#include "SerialPort.h"
#include <cstdio>
#include <iostream>
#include <chrono>
#include <ctime>

// LOOP_PERIOD is in seconds
#define LOOP_PERIOD .05
#define PORT "12345"
#define COMM_PACKET_LENGTH 6
#define SERIAL_RESPONSE_LENGTH 32

SerialPort ser;
SocketServer sock(PORT);
clock_t lasttime;
char *sockData = new char[MAX_DATA_LENGTH], *serData = new char[MAX_DATA_LENGTH];
bool go = true;
int debugLevel = 1;

void transmissionLoop() {
	if (sock.available() >= 4) {
		int sockDataLength = sock.readUntil(sockData, MAX_DATA_LENGTH, '\r');
		if (debugLevel >= 2) {
			printf("Socket: %d | ", sockDataLength);
			for (int i = 0; i < sockDataLength; i++) {
				printf("%d ", sockData[i]);
			}
			printf("\n"); 
		}
		if (sockDataLength == COMM_PACKET_LENGTH) {
			sockData[sockDataLength] = '\0';
			if (strcmp(sockData, "quit") == 0) {
				go = false;
			}
			ser.write(sockData, COMM_PACKET_LENGTH);
		}
	}
	if (ser.available() > SERIAL_RESPONSE_LENGTH) {
		int serDataLength = ser.readUntil(serData, MAX_DATA_LENGTH, '\r');
		if (debugLevel >= 2) {
			printf("Serial: %d | ", serDataLength);
			for (int i = 0; i < serDataLength; i++) {
				printf("%c", serData[i]);
			}
			printf("\n");
		}
		sock.write(serData, serDataLength);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		debugLevel = atoi(argv[1]);
	}
	while (!ser.isConnected() && !sock.isConnected());
	ser.begin();
	sock.begin();

	lasttime = clock();
	while (go) {
		if ((clock()-lasttime+.0)/CLOCKS_PER_SEC > LOOP_PERIOD) {
			transmissionLoop(); 
			lasttime = clock();
		} else {
			std::this_thread::sleep_for(std::chrono::duration<double>(LOOP_PERIOD-(clock()-lasttime+ .0)/CLOCKS_PER_SEC));
			//printf("%f ", time(NULL) - lasttime);
		}
		
	}
	printf("Quitting");
	sock.terminate();
	ser.terminate();
	delete[] sockData;
	delete[] serData;
    return 0;
}


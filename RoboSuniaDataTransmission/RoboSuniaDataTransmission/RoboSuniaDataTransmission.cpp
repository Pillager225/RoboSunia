// RoboSuniaDataTransmission.cpp : Defines the entry point for the console application.
#include "SocketServer.h"
#include "SerialPort.h"
#include <cstdio>

#define PORT "12345"

SerialPort ser;
SocketServer sock(PORT);

int main() {
	char *sockData = new char[MAX_DATA_LENGTH], *serData = new char[MAX_DATA_LENGTH];

	while (!ser.isConnected() && !sock.isConnected());
	ser.begin();
	sock.begin();

	while (true) {
		if (sock.available() >= 4) {
			int sockDataLength = sock.readUntil(sockData, MAX_DATA_LENGTH, '\r');
			//printf("Socket: %d | ", sockDataLength);
			//for (int i = 0; i < sockDataLength; i++) {
			//	printf("%d ", sockData[i]);
			//}
			//printf("\n");
			if(sockDataLength == 4) {
				ser.write(sockData, 4);
			}
		}
		if (ser.available() > 32) {
			int serDataLength = ser.readUntil(serData, MAX_DATA_LENGTH, '\r');
			//printf("Serial: %d | ", serDataLength);
			//for (int i = 0; i < serDataLength; i++) {
			//	printf("%c", serData[i]);
			//}
			//printf("\n");
			sock.write(serData, serDataLength);
		}
	}
	delete[] sockData;
	delete[] serData;
    return 0;
}


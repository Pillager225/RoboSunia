// RoboSuniaDataTransmission.cpp : Defines the entry point for the console application.
#include <cstdio>
#include <csignal>
#include "SocketServer.h"
#include "SerialPort.h"

#define PORT "12345"

class RoboSuniaDataTransmission {
	SerialPort ser = NULL;
	SocketServer sock = NULL;
	char *sockData, *serData;

	void run() {
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
	}
public:
	RoboSuniaDataTransmission() {
		sockData = new char[MAX_DATA_LENGTH];
		serData = new char[MAX_DATA_LENGTH];
		ser = SerialPort();
		Sleep(5000);
		sock = SocketServer(PORT);
		Sleep(5000);
		while (!ser.isConnected() &&!sock.isConnected());
		ser.begin();
		sock.begin();
		run();
	}

	~RoboSuniaDataTransmission() {
		terminate();
	}

	void terminate() {
		ser.terminate();
		sock.terminate();
		delete[] sockData;
		delete[] serData;
	}
};

static void signalHandler(int signum) {
	printf("Signal handler started.\n");
	//r->terminate();
}

int main() {
	RoboSuniaDataTransmission *r = new RoboSuniaDataTransmission();
//	signal(SIGTERM, signalHandler);
//	signal(SIGINT,  signalHandler);
    return 0;
}


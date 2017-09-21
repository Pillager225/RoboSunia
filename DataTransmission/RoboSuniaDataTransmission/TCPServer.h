#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "CommConnection.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class TCPServer : public CommConnection {
protected:
	SOCKET listenSocket, clientSocket;

	bool setupTCPServer(const char *port);
	bool waitForClientConnection();
	void exitGracefully();

	void failedRead();
	int getData(char *buff, const int &buffSize);
public:
	TCPServer(const char *port);
	~TCPServer();
	
	bool write(char *buff, const int &buffSize);
};

#endif 
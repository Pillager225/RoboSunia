#include "SocketServer.h"

// protected
bool SocketServer::setupSocketServer(const char *port) {
	WSADATA wsaData;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return false;
	}
	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}
	// setup socket
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
	    printf("Error at socket(): %ld\n", WSAGetLastError());
	    freeaddrinfo(result);
	    freeaddrinfo(ptr);
	    WSACleanup();
	    return false;
	}	
	BOOL optVal = TRUE;
	iResult = setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &optVal, sizeof(BOOL));
	if (iResult == SOCKET_ERROR) {
		printf("getsockopt for TCP_NODELAY failed with error: %u\n", WSAGetLastError());
	}
	// Setup the TCP listening socket
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        freeaddrinfo(ptr);
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }
    // frees up memory related to connection information
    freeaddrinfo(result);
    freeaddrinfo(ptr);
	return waitForClientConnection();
}

bool SocketServer::acceptClient() {
	clientSocket = accept(listenSocket, NULL, NULL);
	if(clientSocket == INVALID_SOCKET) {
		printf(" accept failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return false;
	} else {
		printf(" Controller client connected!\n");
		connected = true;
		return true;
	}
}

bool SocketServer::acceptClientWrapper() {
	std::mutex m;
	std::condition_variable cv;
	bool retval;
	std::thread t([&m, &cv, &retval, this]()
	{
		retval = acceptClient();
		cv.notify_one();
	});

	t.detach();

	{
		std::unique_lock<std::mutex> l(m);
		if (cv.wait_for(l, std::chrono::seconds(1)) == std::cv_status::timeout)
			throw std::runtime_error("Timeout");
	}

	return retval;
}

bool SocketServer::waitForClientConnection() {
	// setup listening on socket for connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
	    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
	    closesocket(listenSocket);
	    WSACleanup();
	    return false;
	}
	printf("Waiting for controller client connection");
	try {
		return acceptClientWrapper();
	} catch (const std::runtime_error &e) {
		while (!interruptRead && !connected) {
			printf(".");
			Sleep(1000);
		}
		if(interruptRead && !connected) {
			return false;
		} else if(!interruptRead && connected) {
			return true;
		}
	}
}

void SocketServer::exitGracefully() {
	bool safeToClose = true;
	// shutdown the send half of the connection since no more data will be sent
	int iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != WSAENOTSOCK) {
			printf("shutdown failed: %d\n", error);
		}
		safeToClose = false;
	}
	// cleanup
	if (safeToClose) {
		closesocket(clientSocket);
	}
	closesocket(listenSocket);
	WSACleanup();
}

int SocketServer::getData(char *buff, const int &buffSize) {
	if(connected)
		return recv(clientSocket, buff, buffSize, 0);
	else 
		return -1;
}

void SocketServer::failedRead() {
	closesocket(clientSocket);
    connected = false;
	waitForClientConnection();
}

// public 
SocketServer::SocketServer(const char *port) : CommConnection() {
	if(!setupSocketServer(port)) {
		printf("Could not setup socket server. Check parameters.\n");
	}
}

SocketServer::~SocketServer() {
	CommConnection::~CommConnection();
	exitGracefully();
}

bool SocketServer::write(char *buff, const int &buffSize) {
	if(!connected) 
		return false;
	int iSendResult = send(clientSocket, buff, buffSize, 0);
	if(iSendResult == SOCKET_ERROR) {
		printf("Failed to send: %s", buff);
		return false;
	}
	return true;
}

// void SocketServer::echo() {
// 	int iResult, iSendResult;
// 	char recvbuf[MAX_DATA_LENGTH];
// 	// Receive until the peer shuts down the connection
// 	do {
// 	    iResult = recv(clientSocket, recvbuf, MAX_DATA_LENGTH, 0);
// 	    if (iResult > 0) {
// 	        printf("Bytes received: %d\n", iResult);

// 	        // Echo the buffer back to the sender
// 	        iSendResult = send(clientSocket, recvbuf, iResult, 0);
// 	        if (iSendResult == SOCKET_ERROR) {
// 	            printf("send failed: %d\n", WSAGetLastError());
// 	            closesocket(clientSocket);
// 	            isConnected = false;
// 	        	waitForClientConnection();
// 	        }
// 	        printf("Bytes sent: %d\n", iSendResult);
// 	    } else if (iResult == 0) {
// 	        printf("Connection closing...\n");
// 	    } else {
// 	        printf("recv failed: %d\n", WSAGetLastError());
// 	        closesocket(clientSocket);
// 	        isConnected = false;
// 	        waitForClientConnection();
// 	    }

// 	} while (iResult > 0);
// 	exitGracefully();
// }
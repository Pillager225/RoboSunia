#include <iostream>

char *buffer;
int readIndex = 0;

int readUntil(char *buff, const int &buffSize, const char &delim) { // TODO this needs work! Can't handle wrap around case
	int i = readIndex, count = 0, leftOff = 0;
	while(true) {
		if(available() > 0) {
			if(buffer[i] == delim) {
				memcpy_s(&buff[leftOff], buffSize-leftOff, &buffer[readIndex], i-readIndex);
				readIndex = i+1;
				return count;
			} else if(i+1 == BUFFER_SIZE) {
				memcpy_s(&buff[leftOff], buffSize-leftOff, &buffer[readIndex], i-readIndex);
				leftOff = i-readIndex;
				readIndex = 0;
				i = 0;
			} else {
				i++;
			}
			count++;
			if(count == buffSize) {
				return count;
			}
		}
	}
}

int main() {
	buffer = "123\r5678\r90\r1";
	char *buff = new char[16];
	int read = readUntil(buff, 16, '\r');
	std::cout << "read: " << buff << endl;
	read = readUntil(buff, 16, '\r');
	std::cout << "read: " << buff << endl;
	int read = readUntil(buff, 16, '\r');
	std::cout << "read: " << buff << endl;
	int read = readUntil(buff, 16, '\r');
	std::cout << "read: " << buff << endl;
}
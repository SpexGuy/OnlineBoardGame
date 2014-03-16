#ifdef _WIN32
#include <assert.h>
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"

using namespace std;

WSAData socketData;

void SocketInit() {
	if (WSAStartup(MAKEWORD(2,0), &socketData) != 0) {
		cout << "Failed to start WSA" << endl;
		assert(false);
	}
}

void SocketClose() {
	WSACleanup();
}


Socket::Socket(string ip, short int port) {
	socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		cerr << "Could not create socket" << endl;
		assert(false);
	}

	struct sockaddr target;
	struct sockaddr_in *target4 = (sockaddr_in *) &target;

	target4->sin_family = AF_INET;
	target4->sin_port = htons(port);
	target4->sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	memset(target4->sin_zero, 0, sizeof(target4->sin_zero));

	if (connect(socketFD, &target, sizeof(target)) < 0) {
		cout << "Unable to connect!" << endl;
		assert(false);
	}
}

void Socket::sendRawBytes(char *byteData, int size) {
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesSent = send(socketFD, byteData, bytesLeft, 0);
		if (bytesSent < 0) {
			cout << "Failed to send bytes" << endl;
			assert(false);
		}
		byteData += bytesSent;
		bytesLeft -= bytesSent;
	}
}

void Socket::readRawBytes(const void *data, int size) {
	char *byteData = (char *) data;
	int bytesLeft = size;
	while(bytesLeft > 0) {
		int bytesRecv = recv(socketFD, byteData, bytesLeft, 0);
		if (bytesRecv < 0) {
			cout << "Could not receive bytes" << endl;
			assert(false);
		}
		if (bytesRecv == 0) {
			cout << "Disconnection!" << endl;
			return;
		}
		byteData += bytesRecv;
		bytesLeft -= bytesRecv;
	}
}

void Socket::sendData(int type, const void *data, int size) {
	int ntype = htonl(type);
	int nsize = htonl(size);
	sendRawBytes((char *) &ntype, sizeof(ntype));
	sendRawBytes((char *) &nsize, sizeof(nsize));
	sendRawBytes((char *) data, size);
}

SerialData Socket::receive() {
	int type;
	readRawBytes(&type, sizeof(type));
	type = ntohl(type);
	
	int size;
	readRawBytes(&size, sizeof(size));
	size = ntohl(size);

	char *data = (char *) malloc(size);
	if (data == NULL) {
		cout << "Could not malloc " << size << " bytes!" << endl;
		assert(false);
	}
	readRawBytes(data, size);
	
	SerialData ret;
	ret.data = data;
	ret.size = size;
	ret.type = type;
	return ret;
}




Socket::~Socket() {
	closesocket(socketFD);
}





#endif

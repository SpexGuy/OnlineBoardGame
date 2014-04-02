#ifdef _WIN32
#include <assert.h>
#include <iostream>
#include <WinSock2.h>
#include "UDPSocket.h"

using namespace std;

UDPSocket::UDPSocket() :
	socketFD(0)
{}

bool UDPSocket::connect(string ip, uint16_t port) {
	assert(!isOpen());
	socketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketFD <= 0) {
		cerr << "Could not create socket" << endl;
		socketFD = 0;
		return false;
	}

	struct sockaddr target;
	struct sockaddr_in *target4 = (sockaddr_in *) &target;

	target4->sin_family = AF_INET;
	target4->sin_port = htons(port);
	target4->sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	memset(target4->sin_zero, 0, sizeof(target4->sin_zero));

	if (::connect(socketFD, &target, sizeof(target)) < 0) {
		cout << "Unable to connect!" << endl;
		close();
		return false;
	}
	return true;
}

bool UDPSocket::connect(uint32_t ip, uint16_t port) {
	assert(!isOpen());
	socketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketFD <= 0) {
		cerr << "Could not create socket" << endl;
		socketFD = 0;
		return false;
	}

	struct sockaddr target;
	struct sockaddr_in *target4 = (sockaddr_in *) &target;

	target4->sin_family = AF_INET;
	target4->sin_port = htons(port);
	target4->sin_addr.S_un.S_addr = htonl(ip);
	memset(target4->sin_zero, 0, sizeof(target4->sin_zero));

	if (::connect(socketFD, &target, sizeof(target)) < 0) {
		cout << "Unable to connect!" << endl;
		close();
		return false;
	}
	return true;
}

int UDPSocket::send(const uint8_t *data, int len) {
	assert(len <= MAX_UDP_SIZE);
	return ::send(socketFD, (const char *)data, len, 0);
}

int UDPSocket::receive(uint8_t *data, int maxLen) {
	return ::recv(socketFD, (char *)data, maxLen, 0);
}

void UDPSocket::close() {
	if (isOpen()) {
		closesocket(socketFD);
		socketFD = 0;
	}
}

#endif

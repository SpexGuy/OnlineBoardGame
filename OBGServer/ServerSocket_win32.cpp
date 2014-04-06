#ifdef _WIN32
#include <assert.h>
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "ServerSocket.h"


using namespace std;

ServerSocket::ServerSocket() :
	socketFD(0)
{}

bool ServerSocket::open(uint16_t port) {
	//ugly code be here; read at your peril
	//create the socket
	socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFD <= 0) {
		cout << "Could not create server socket" << endl;
		socketFD = 0;
		return false;
	}
	//create a sockaddr to bind to localhost
	struct sockaddr localhost;
	//convert it to ipv4 to safely set the data
	struct sockaddr_in *local4 = (sockaddr_in *) &localhost;
	local4->sin_family = AF_INET;
	//set the port (network byte order)
	local4->sin_port = htons(port);
	//set the ipv4 part of the address to localhost
	local4->sin_addr.s_addr = INADDR_ANY;
	//zero the rest of the address
	memset(local4->sin_zero, 0, sizeof(local4->sin_zero));
	//bind to localhost
	bind(socketFD, &localhost, sizeof(localhost));
	//set up listen queue with size 5
	listen(socketFD, 5);
	return true;
}

Socket *ServerSocket::getNextConnection() {
	struct sockaddr_in newInfo;
	int size = sizeof(newInfo);
	int newFD = accept(socketFD, (sockaddr *)&newInfo, &size);
	if (newFD < 0) {
		cout << "Accept had an error!" << endl;
		return NULL;
	}
	assert(newInfo.sin_family == AF_INET);
	Address client = Address::TCPAddress(ntohl(newInfo.sin_addr.s_addr), ntohs(newInfo.sin_port));

	return new Socket(newFD, client);
}

void ServerSocket::close() {
	if (isOpen()) {
		closesocket(socketFD);
		socketFD = 0;
	}
}

ServerSocket::~ServerSocket() {
	close();
}

#endif

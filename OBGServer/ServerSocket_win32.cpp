#ifdef _WIN32
#include <assert.h>
#include <iostream>
#include <WinSock2.h>
#include "Socket.h"
#include "ServerSocket.h"


using namespace std;

ServerSocket::ServerSocket(short int port) {
	//ugly code be here; read at your peril
	//create a sockaddr to bind to localhost
	struct sockaddr localhost;
	//convert it to ipv4 to safely set the data
	struct sockaddr_in *local4 = (sockaddr_in *) &localhost;
	local4->sin_family = AF_INET;
	//set the port (network byte order)
	local4->sin_port = htons(port);
	//set the ipv4 part of the address to 127.0.0.1
	local4->sin_addr.S_un.S_addr = INADDR_ANY;
	//zero the rest of the address
	memset(local4->sin_zero, 0, sizeof(local4->sin_zero));
	//create the socket
	socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		cout << "Could not create socket" << endl;
		assert(false);
	}
	//bind to localhost
	bind(socketFD, &localhost, sizeof(localhost));
	//set up listen queue with size 5
	listen(socketFD, 5);
}

Socket *ServerSocket::getNextConnection() {
	struct sockaddr newInfo;
	int size = sizeof(newInfo);
	int newFD = accept(socketFD, &newInfo, &size);
	if (newFD < 0) {
		cout << "Accept had an error!" << endl;
		assert(false);
	}

	return new Socket(newFD);
}

ServerSocket::~ServerSocket() {
	closesocket(socketFD);
}



#endif
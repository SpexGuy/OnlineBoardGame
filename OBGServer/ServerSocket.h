#pragma once

class Socket;

class ServerSocket {
private:
	ServerSocket();
protected:
	int socketFD;
public:
	ServerSocket(short int port);
	virtual Socket *getNextConnection();
	virtual ~ServerSocket();
};

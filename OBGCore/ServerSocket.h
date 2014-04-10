#pragma once
#include <stdint.h>

class Socket;

class ServerSocket {
protected:
	int socketFD;
public:
	ServerSocket();
	virtual bool open(uint16_t port);
	virtual Socket *getNextConnection();
	virtual void close();
	virtual ~ServerSocket();

	inline bool isOpen() { return socketFD != 0; }
};

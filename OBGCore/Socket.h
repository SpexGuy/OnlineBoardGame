#pragma once
#include <string>

void SocketInit();

void SocketClose();

class SerialData {
public:
	int   type;
	int   size;
	void *data;
};

class Socket {
private:
	void sendRawBytes(char *data, int size);
	void readRawBytes(const void *data, int size);
protected:
	int socketFD;
public:
	Socket(int fd) :
		socketFD(fd) {}
	Socket(std::string ip, short int port);

	virtual void sendData(int type, const void *data, int size);

	/** blocks for input, then returns that input */
	virtual SerialData receive();

	virtual ~Socket();
};

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
	int sendRawBytes(char *data, int size);
	int readRawBytes(const void *data, int size);
protected:
	int socketFD;
public:
	Socket(int fd) :
		socketFD(fd) {}
	Socket(std::string ip, short int port);

	/**	returns 0 if the data is send successfully, or the number
	 *	of bytes which are left to send if a fatal error occurs. */
	virtual int sendData(int type, const void *data, int size);

	/**	Blocks for input, then returns that input.
	 *	If a fatal error occurs, the returned
	 *	SerialData's data pointer will be NULL.*/
	virtual SerialData receive();

	virtual ~Socket();
};

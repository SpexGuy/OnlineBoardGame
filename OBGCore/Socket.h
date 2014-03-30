#pragma once
#include <string>

#define TYPE_FILE -1

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
	bool connected;
public:
	Socket(int fd, bool connected = true) :
		socketFD(fd), connected(connected) {}
	Socket(const std::string &ip, short int port);

	/**	returns 0 if the data is send successfully, or the number
	 *	of bytes which are left to send if a fatal error occurs.
	 *	negative types are reserved; type must be positive.*/
	virtual int sendData(int type, const void *data, int size);

	virtual int sendFile(const std::string &filename);

	/**	Blocks for input, then returns that input.
	 *	If a fatal error occurs, the returned
	 *	SerialData's data pointer will be NULL.*/
	virtual SerialData receive();

	virtual ~Socket();
};

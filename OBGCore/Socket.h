#pragma once
#include <string>
#include "Address.h"

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
	Address peer;
	int socketFD;
public:
	Socket(int fd, const Address &peer) :
		socketFD(fd),
		peer(peer)
	{}
	Socket();

	virtual bool open(const Address &peer, int localPort = 0);

	/**	returns 0 if the data is send successfully, or the number
	 *	of bytes which are left to send if a fatal error occurs.
	 *	negative types are reserved; type must be positive.*/
	virtual int sendData(int type, const void *data, int size);

	virtual int sendFile(const std::string &filename);

	/**	Blocks for input, then returns that input.
	 *	If a fatal error occurs, the returned
	 *	SerialData's data pointer will be NULL.*/
	virtual SerialData receive();

	virtual void close();

	virtual ~Socket();

	inline const Address &getPeer() const { return peer; }
	inline bool isOpen() const { return socketFD != 0; }
};

#pragma once
#include <string>
#include "Address.h"

#define TYPE_FILE -1

void SocketInit();

void SocketClose();

#pragma warning(disable:4200)
struct SerialData {
	uint8_t  type;
	uint16_t size;
	uint8_t  data[0];
};
#pragma warning(default:4200)

class Socket {
protected:
	Address peer;
	int socketFD;
public:
	int sendRawBytes(const uint8_t *data, uint16_t size);
	int readRawBytes(uint8_t *data, uint16_t size);

	Socket(int fd, const Address &peer) :
		socketFD(fd),
		peer(peer)
	{}
	Socket();

	virtual bool open(const Address &peer, uint16_t localPort = 0);

	/**	returns 0 if the data is send successfully, or the number
	 *	of bytes which are left to send if a fatal error occurs.
	 *	negative types are reserved; type must be positive.*/
	virtual int sendData(uint8_t type, const uint8_t *data, uint16_t size);

	virtual int sendFile(const std::string &filename);

	/**	Blocks for input, then returns that input.
	 *	If a fatal error occurs, the returned
	 *	SerialData's data pointer will be NULL.*/
	virtual int receive(uint8_t &type, uint8_t *data, uint16_t maxSize);

	virtual void close();

	virtual ~Socket();

	inline const Address &getPeer() const { return peer; }
	inline bool isOpen() const { return socketFD != 0; }
};

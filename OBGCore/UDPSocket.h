#pragma once
#include <stdint.h>
#include <string>
#include "Address.h"

#define ANY_PORT 0
#define MAX_UDP_SIZE 65507

void sleep(int millis);

class UDPSocket {
private:

protected:
	int socketFD;

public:
	UDPSocket();
	bool open(uint16_t localPort = ANY_PORT);
	bool connect(std::string ip, uint16_t port);
	bool connect(uint32_t ip, uint16_t port);
	int  send(const Address &to, const uint8_t *data, int len);
	int  receive(Address &from, uint8_t *data, int maxLen);
	void close();
	~UDPSocket();

	inline bool isOpen() const { return socketFD != 0; }
};

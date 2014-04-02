#pragma once
#include <stdint.h>
#include <string>

#define MAX_UDP_SIZE 65507

class UDPSocket {
private:

protected:
	int socketFD;

public:
	UDPSocket();
	bool connect(std::string ip, uint16_t port);
	bool connect(uint32_t ip, uint16_t port);
	int  send(const uint8_t *data, int len);
	int  receive(uint8_t *data, int maxLen);
	void close();
	~UDPSocket();

	inline bool isOpen() { return socketFD != 0; }
};

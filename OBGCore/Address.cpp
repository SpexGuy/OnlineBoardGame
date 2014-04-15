#include "Address.h"

Address::Address() {
	address = 0;
	port = 0;
}

Address::Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) {
	this->address = (a << 24) | (b << 16) | (c << 8) | d;
	this->port = port;
}

Address::Address(uint32_t address, uint16_t port) {
	this->address = address;
	this->port = port;
}

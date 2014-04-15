#pragma once
#include <stdint.h>
#include <string>

class Address {
private:
	Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
	Address(uint32_t address, uint16_t port);

	uint32_t address;
	uint16_t port;
public:
	Address();
	static inline Address TCPAddress(uint32_t address, uint16_t port) {
		return Address(address, port);
	}
	static inline Address TCPAddress(const std::string &address, uint16_t port) {
		uint32_t a,b,c,d;
		if (sscanf_s(address.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d )) {
			return Address(a,b,c,d,port);
		} else {
			return Address();
		}
	}
	static inline Address UDPAddress(uint32_t address, uint16_t port) {
		return Address(address, port - 1);
	}

	inline uint32_t GetAddress() const { return address; }
	inline uint8_t GetA() const { return uint8_t(address >> 24); }
	inline uint8_t GetB() const { return uint8_t(address >> 16); }
	inline uint8_t GetC() const { return uint8_t(address >> 8 ); }
	inline uint8_t GetD() const { return uint8_t(address); }
	inline uint16_t GetTCPPort() const { return port; }
	inline uint16_t GetUDPPort() const { return port + 1; }
	
	bool operator == (const Address & other) const {
		return address == other.address && port == other.port;
	}
	
	bool operator != (const Address & other) const {
		return ! (*this == other);
	}

	operator bool() {
		return address != 0;
	}
		
	bool operator < (const Address & other) const {
		// note: this is so we can use address as a key in std::map
		if (address < other.address)
			return true;
		if (address > other.address)
			return false;
		return port < other.port;
	}
};

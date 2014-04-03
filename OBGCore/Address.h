#pragma once

class Address {
private:
	unsigned int address;
	unsigned short port;
public:
	Address();
	Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
	Address(unsigned int address, unsigned short port);
	
	inline unsigned int GetAddress() const { return address; }
	inline unsigned char GetA() const { return (unsigned char) (address >> 24); }
	inline unsigned char GetB() const { return (unsigned char) (address >> 16); }
	inline unsigned char GetC() const { return (unsigned char) (address >> 8 ); }
	inline unsigned char GetD() const { return (unsigned char) (address); }
	inline unsigned short GetPort() const { return port; }
	
	bool operator == (const Address & other) const {
		return address == other.address && port == other.port;
	}
	
	bool operator != (const Address & other) const {
		return ! (*this == other);
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

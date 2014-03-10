#pragma once
#include <string>


class Socket {
public:
	virtual void connect(std::string ipAddr);
	virtual void connect(std::string ip, short int port);
	virtual void connect(int ip, short int port);

	virtual void send(std::string message);

	/** blocks for input, then returns that input */
	virtual std::string receive();

	virtual void close();

	virtual ~Socket();
};

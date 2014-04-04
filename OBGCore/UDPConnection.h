/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
	Adapted by Martin Wickham <martin@wikiwickham.com>
*/
#pragma once
#include <map>
#include <set>
#include "Address.h"
#include "ReliabilitySystem.h"
#include "Thread.h"
#include "UDPSocket.h"

#define MAX_SEQNO 0xFFFF
typedef uint16_t seqno_t;

struct Message {
	uint8_t  type;			//The message type
	uint8_t  data[0];		//The payload
};

struct Packet {
	uint32_t protocol;		//The unique protocol id
	seqno_t  seqno;			//The packet number
	seqno_t  ack;			//The most recently received packet
	uint32_t ack_bits;		//The bitmask for previous packets
	Message  message;		//The message
};

class UDPClientListener {
public:
	virtual void handleMessage(int type, uint8_t *data, int len) = 0;
	virtual ~UDPClientListener() {}
};

class UDPServerListener {
public:
	virtual void handleMessage(const Address &from, int type, uint8_t *data, int len) = 0;
	virtual bool handleUnresponsiveClient(const Address &client);
	virtual ~UDPServerListener() {}
};

class UDPClient {
protected:
	unsigned int protocolId;
	int timeout;
	UDPClientListener *listener;

	UDPSocket socket;
	int lastMessageTime;
	int time;

	Address server;
	ReliabilitySystem reliabilitySystem;

	Thread thread;

public:
	UDPClient(uint32_t protocolId, int timeout, UDPClientListener *listener);
	virtual bool start(int time, const Address &server, uint16_t localPort = ANY_PORT);
	virtual bool update(int time);
	virtual bool sendPacket(int type, int len, const uint8_t *data);
	virtual int  loop();
	virtual void stop();

	inline bool isRunning() const { return socket.isOpen(); }
	inline bool isUnresponsive() const { return time - lastMessageTime > timeout; }
};

class UDPServer {
private:
	struct ClientInfo {
		ReliabilitySystem reliabilitySystem;
		int lastMessageTime;
	};
	std::map<Address, ClientInfo> clients;
	UDPServerListener *listener;
	int time;
	UDPSocket socket;
	Thread thread;

	unsigned int protocolId;
	int timeout;

public:
	UDPServer(uint32_t protocolId, int timeout, UDPServerListener *listener);
	virtual void addClient(const Address &client);
	virtual bool start(int time, uint16_t port);
	virtual bool update(int time);
	virtual bool broadcastPacket(int type, int size, const uint8_t *data, const Address &ignore = Address(0,0));
	virtual bool sendPacket(const Address &destination, int type, int size, const uint8_t *data);
	virtual bool sendUnindexedPacket(const Address &destination, int type, int size, const uint8_t *data);
	virtual int  loop();
	virtual void stop();

	bool isUnresponsive(const Address &addr);

	inline bool isRunning() const { return socket.isOpen(); }
};



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
#include "UDPSocket.h"

#define TYPE_CONNECT_REQUEST 1
#define TYPE_CONNECT_ACCEPT 2
#define TYPE_CONNECT_DECLINE 3

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

class UDPClient {
protected:
	enum State {
		Disconnected,
		Connecting,
		ConnectFail,
		Connected
	};

	unsigned int protocolId;
	int timeout;

	bool running;
	State state;
	UDPSocket socket;
	int lastMessageTime;
	int time;

	Address server;
	ReliabilitySystem reliabilitySystem;

	virtual bool preHandlePacket(Packet *pack);
public:
	UDPClient(uint32_t protocolId, int timeout);
	virtual bool connect(const Address &server, int attemptMillis = 5000, int packetSendRate = 50);
	virtual bool start(int time, uint16_t port = ANY_PORT);
	virtual bool update(int time);
	virtual bool sendPacket(int type, int len, const uint8_t *data);
	virtual int receivePacket(int &type, int len, uint8_t *data);
	virtual void stop();

	inline bool isConnecting() const { return state == Connecting; }
	inline bool connectFailed() const { return state == ConnectFail; }
	inline bool isConnected() const { return state == Connected; }
	inline bool isRunning() const { return running; }
	inline bool isUnresponsive() const { return time - lastMessageTime > timeout; }
};

class UDPServer {
private:
	struct ClientInfo {
		ReliabilitySystem reliabilitySystem;
		int lastMessageTime;
	};
	std::map<Address, ClientInfo> clients;
	std::set<Address> declined;
	int time;
	UDPSocket socket;
	bool running;

	unsigned int protocolId;
	int timeout;

protected:
	virtual void handleUnknownClient(const Address &addr, Packet *request, int len);
	virtual bool acceptNewPlayer(const Address &addr, Packet *request, int len);
	virtual bool preHandlePacket(const Address &addr, Packet *pack, int len);

	virtual void onConnect(const Address &player) {}
	virtual void onDisconnect(const Address &player) {}

public:
	UDPServer(uint32_t protocolId, int timeout);
	virtual bool start(int time, uint16_t port);
	virtual bool update(int time);
	virtual bool broadcastPacket(int type, int size, const uint8_t *data);
	virtual bool sendPacket(const Address &destination, int type, int size, const uint8_t *data);
	virtual bool sendUnindexedPacket(const Address &destination, int type, int size, const uint8_t *data);
	virtual int  receivePacket(Address &from, int &type, int size, uint8_t *data);
	virtual void stop();

	bool isUnresponsive(const Address &addr);
};



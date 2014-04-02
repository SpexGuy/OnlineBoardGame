#pragma once
#include "UDPSocket.h"

#define ANY_PORT 0
#define MAX_SEQNO 0xFFFF
typedef uint16_t seqno_t;

struct Packet {
	uint32_t protocol;		//The unique protocol id
	seqno_t seqno;			//The packet number
	seqno_t ack;			//The most recently received packet
	uint32_t ack_bits;		//The bitmask for previous packets
};

class UDPConnection {
private:
	enum State {
		Disconnected,
		Listening,
		Connecting,
		ConnectFail,
		Connected
	};

	unsigned int protocolId;
	float timeout;
		
	bool running;
	State state;
	UDPSocket socket;
	float timeoutAccumulator;
	Address address;

	void ClearData();

protected:
	virtual void OnStart()		{}
	virtual void OnStop()		{}
	virtual void OnConnect()    {}
	virtual void OnDisconnect() {}
			
public:
	UDPConnection(unsigned int protocolId, float timeout);
	bool start(int port = ANY_PORT);
	void listen();
	void connect(const Address & address);
	virtual void update(float deltaTime);
	virtual bool sendPacket(const unsigned char data[], int size);
	virtual int receivePacket(unsigned char data[], int size);
	void stop();
	virtual ~UDPConnection();
		
	inline int getHeaderSize() const { return 4; }
	inline bool isConnecting() const { return state == Connecting; }
	inline bool connectFailed() const { return state == ConnectFail; }
	inline bool isConnected() const { return state == Connected; }
	inline bool isListening() const { return state == Listening; }
	inline bool isRunning() const { return running; }
};

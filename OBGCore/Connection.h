#pragma once
#include "Socket.h"
#include "Thread.h"

#define TYPE_PHYSICS_UPDATE 1
#define TYPE_INTERACTION 2
#define TYPE_MESSAGE 3
#define TYPE_SET_USERNAME 4
#define TYPE_FILE_REQUEST 5

class Connection {
private:
	Connection();

protected:
	Socket *socket;
	Thread runThread;

	volatile bool active;

	virtual int  sendFile(const std::string &name);
	virtual void processData(const SerialData &data) = 0;
	virtual void handleFatalError() = 0;
public:
	Connection(Socket *socket);
	virtual void start();
	virtual void loop();
	virtual void close();
	virtual ~Connection();
	
	inline bool isActive()	{ return active; }
	inline const Address &getPeer() const { return socket->getPeer(); }
};

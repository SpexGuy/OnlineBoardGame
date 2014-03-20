#pragma once

#define TYPE_PHYSICS_UPDATE 1
#define TYPE_INTERACTION 2
#define TYPE_MESSAGE 3
#define TYPE_SET_USERNAME 4

class Socket;
class SerialData;

class Connection {
private:
	Connection();
	volatile bool loopEnded;

protected:
	Socket *socket;
	volatile bool active;

	volatile unsigned long threadId;
	volatile void *thread;

	virtual void processData(const SerialData &data) = 0;
	virtual void handleFatalError() = 0;
public:
	Connection(Socket *socket);
	virtual void start();
	virtual void loop();
	//TODO:[MW] move this inside of (Server|Client)Connection
	virtual int  sendFile(const std::string &name);
	virtual void close();
	virtual ~Connection();
	
	inline bool isActive()	{ return active; }
};

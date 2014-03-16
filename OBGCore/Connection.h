#pragma once

#define TYPE_PHYSICS_UPDATE 1
#define TYPE_INTERACTION 2
#define TYPE_MESSAGE 3

class Socket;
class SerialData;

class Connection {
private:
	Connection();
protected:
	Socket *socket;
	volatile bool active;

	volatile unsigned long threadId;
	volatile void *thread;

	virtual void processData(const SerialData &data) = 0;
public:
	Connection(Socket *socket);
	virtual void start();
	virtual void loop();
	virtual void close();
	virtual ~Connection();
	
	inline bool isActive()	{ return active; }
};

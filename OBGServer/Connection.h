#pragma once
#include <string>

class Socket;
class PlayerManager;
class PhysicsUpdate;

class Connection {
protected:
	Socket *socket;
	volatile long int lastCommTime;
	PlayerManager *manager;

public:
	virtual void start();
	virtual bool isUnresponsive();
	virtual void sendUpdate(PhysicsUpdate *update);
	virtual void sendMessage(const std::string &message);
	virtual void close();
	virtual ~Connection();
};

#pragma once
#include <string>
#include <Connection.h>

class Socket;
class PlayerManager;
class PhysicsUpdate;

class ServerConnection :
	public Connection
{
protected:
	PlayerManager *owner;
	volatile long int lastCommTime;	
	std::string name;

	virtual void processData(const SerialData &data);

public:
	ServerConnection(PlayerManager *owner, Socket *socket);
	virtual bool isUnresponsive();
	virtual void sendUpdate(PhysicsUpdate *update);
	virtual void sendMessage(const std::string &message);
	virtual ~ServerConnection();

	inline std::string getName()		{ return name; }
	inline PlayerManager *getManager()	{ return owner; }
};

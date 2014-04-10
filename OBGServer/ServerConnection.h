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

	virtual void processData(uint8_t type, const uint8_t *data, uint16_t len);
	virtual void handleFatalError();
public:
	ServerConnection(PlayerManager *owner, Socket *socket);
	virtual void sendUpdate(PhysicsUpdate *update);
	virtual void sendMessage(const std::string &message);
	virtual ~ServerConnection();

	inline std::string getName()		{ return name; }
	inline PlayerManager *getManager()	{ return owner; }
};

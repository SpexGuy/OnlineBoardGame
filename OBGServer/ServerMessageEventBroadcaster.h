#pragma once
#include <vector>

class ServerMessageListener;
class Player;

class ServerMessageEventBroadcaster {
private:
	std::vector<ServerMessageListener *> listeners;

protected:
	void fireMessage(const std::string &message, Player *sender);

public:
	ServerMessageEventBroadcaster() {};
	virtual void registerServerMessageListener(ServerMessageListener *listener);
	virtual ~ServerMessageEventBroadcaster() {};
};
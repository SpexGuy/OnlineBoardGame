#pragma once
#include <vector>

class ServerMessageListener;
class Connection;

class ServerMessageEventBroadcaster {
private:
	std::vector<ServerMessageListener *> listeners;

protected:
	void fireMessage(const std::string &message, Connection *sender);

public:
	ServerMessageEventBroadcaster() {};
	virtual void registerServerMessageListener(ServerMessageListener *listener);
	virtual ~ServerMessageEventBroadcaster() {};
};

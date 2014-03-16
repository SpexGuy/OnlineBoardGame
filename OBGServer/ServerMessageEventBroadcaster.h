#pragma once
#include <vector>

class ServerMessageListener;
class ServerConnection;

class ServerMessageEventBroadcaster {
private:
	std::vector<ServerMessageListener *> listeners;

protected:
	void fireMessage(const std::string &message, ServerConnection *sender);

public:
	ServerMessageEventBroadcaster() {};
	virtual void registerServerMessageListener(ServerMessageListener *listener);
	virtual ~ServerMessageEventBroadcaster() {};
};

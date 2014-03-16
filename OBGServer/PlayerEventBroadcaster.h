#pragma once
#include <vector>

class PlayerListener;
class ServerConnection;

class PlayerEventBroadcaster {
private:
	std::vector<PlayerListener *> listeners;

protected:
	void firePlayerJoined(ServerConnection *player);
	void firePlayerLeft(ServerConnection *player);

public:
	PlayerEventBroadcaster() {};
	virtual void registerPlayerListener(PlayerListener *listener);
	virtual ~PlayerEventBroadcaster() {};
};

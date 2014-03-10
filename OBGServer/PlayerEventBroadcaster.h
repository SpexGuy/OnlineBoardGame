#pragma once
#include <vector>

class PlayerListener;
class Player;

class PlayerEventBroadcaster {
private:
	std::vector<PlayerListener *> listeners;

protected:
	void firePlayerJoined(Player *player);
	void firePlayerLeft(Player *player);

public:
	PlayerEventBroadcaster() {};
	virtual void registerPlayerListener(PlayerListener *listener);
	virtual ~PlayerEventBroadcaster() {};
};

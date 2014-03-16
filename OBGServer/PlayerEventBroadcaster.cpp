#include "PlayerEventBroadcaster.h"
#include "PlayerListener.h"

using namespace std;

void PlayerEventBroadcaster::
	firePlayerJoined(ServerConnection *player)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->playerJoined(player);
	}
}

void PlayerEventBroadcaster::
	firePlayerLeft(ServerConnection *player)
{
	for (unsigned int c = 0; c < listeners.size(); c++) {
		listeners[c]->playerLeft(player);
	}
}

void PlayerEventBroadcaster::
	registerPlayerListener(PlayerListener *listener)
{
	listeners.push_back(listener);
}


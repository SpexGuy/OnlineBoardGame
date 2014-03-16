#pragma once
#include "PlayerListener.h"

class PlayerStatusBroadcaster :
	public PlayerListener
{
public:
	virtual void playerJoined(ServerConnection *player);
	virtual void playerLeft(ServerConnection *player);
};

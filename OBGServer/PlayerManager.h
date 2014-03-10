#pragma once
#include <vector>
#include <PhysicsUpdateListener.h>
#include "ServerMessageEventBroadcaster.h"
#include "PlayerEventBroadcaster.h"

class Player;
class ServerSocket;

class PlayerManager :
	public PlayerEventBroadcaster,
	public ServerMessageEventBroadcaster,
	public PhysicsUpdateListener
{
protected:
	std::vector<Player *> players;
	ServerSocket *socket;
public:
	virtual void disconnectPlayer(Player *p);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);
	virtual void broadcast(std::string message, Player *exclude);
};

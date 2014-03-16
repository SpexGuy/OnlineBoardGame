#pragma once
#include <vector>
#include <PhysicsUpdateListener.h>
#include "InteractionListener.h"
#include "PlayerEventBroadcaster.h"
#include "ServerMessageListener.h"
#include "ServerMessageEventBroadcaster.h"

class ServerSocket;
class ServerConnection;

class PlayerManager :
	public InteractionListener,
	public PhysicsUpdateListener,
	public PlayerEventBroadcaster,
	public ServerMessageEventBroadcaster,
	public ServerMessageListener
{
protected:
	std::vector<ServerConnection *> players;
	ServerSocket *socket;
public:
	virtual void start();
	virtual void disconnectPlayer(ServerConnection *p);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);
	virtual void handleInteraction(Interaction *action);
	virtual void handleMessage(const std::string &msg, ServerConnection *sender);
	virtual void broadcast(std::string message, ServerConnection *exclude);
};

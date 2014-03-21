#pragma once
#include <vector>
#include <Mutex.h>
#include <PhysicsUpdateListener.h>
#include <Thread.h>
#include "InteractionEventBroadcaster.h"
#include "InteractionListener.h"
#include "PlayerEventBroadcaster.h"
#include "ServerMessageListener.h"
#include "ServerMessageEventBroadcaster.h"

class ServerSocket;
class ServerConnection;

class PlayerManager :
	public InteractionEventBroadcaster,
	public InteractionListener,
	public PhysicsUpdateListener,
	public PlayerEventBroadcaster,
	public ServerMessageEventBroadcaster,
	public ServerMessageListener
{
private:
	PlayerManager();
protected:
	std::vector<ServerConnection *> players;
	ServerSocket *socket;
	volatile bool active;

	Thread thread;
	Mutex playersMutex;

	virtual void addPlayer(ServerConnection *player);
public:
	PlayerManager(short int port);
	virtual void start();
	virtual void loop();
	virtual void disconnectPlayer(ServerConnection *p);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);
	virtual void handleInteraction(Interaction *action);
	virtual void handleMessage(const std::string &msg, ServerConnection *sender);
	virtual void broadcast(std::string message, ServerConnection *exclude);
	virtual void close();
	virtual ~PlayerManager();
};

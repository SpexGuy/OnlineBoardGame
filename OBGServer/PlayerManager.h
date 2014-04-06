#pragma once
#include <map>
#include <CriticalSection.h>
#include <PhysicsUpdateListener.h>
#include <Thread.h>
#include <UDPConnection.h>
#include "InteractionEventBroadcaster.h"
#include "InteractionListener.h"
#include "PlayerEventBroadcaster.h"
#include "ServerMessageListener.h"
#include "ServerMessageEventBroadcaster.h"
#include "ServerSocket.h"

class ServerConnection;

class PlayerManager :
	public InteractionEventBroadcaster,
	public InteractionListener,
	public PhysicsUpdateListener,
	public PlayerEventBroadcaster,
	public ServerMessageEventBroadcaster,
	public ServerMessageListener,
	public UDPServerListener
{
protected:
	std::map<Address, ServerConnection *> players;
	ServerSocket socket;
	volatile bool active;

	UDPServer udpServer;

	Thread thread;
	CriticalSection playersLock;

	virtual void addPlayer(ServerConnection *player);
public:
	PlayerManager();
	virtual bool start(int time, uint16_t port);
	virtual void loop();
	virtual void update(int time);
	virtual void disconnectPlayer(ServerConnection *p);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);
	virtual void handleInteraction(Interaction *action);
	virtual void handleMessage(const std::string &msg, ServerConnection *sender);
	virtual void handleMessage(const Address &from, int type, uint8_t *data, int len);
	virtual bool handleUnresponsiveClient(const Address &client);
	virtual void broadcast(std::string message, ServerConnection *exclude);
	virtual void close();
	virtual ~PlayerManager();
};

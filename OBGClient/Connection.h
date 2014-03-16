#pragma once
#include <PhysicsUpdateEventBroadcaster.h>
#include <InteractionListener.h>
#include "MessageEventBroadcaster.h"

class Socket;

class Connection :
	public PhysicsUpdateEventBroadcaster,
	public MessageEventBroadcaster,
	public InteractionListener
{
protected:
	Socket *socket;
public:
	virtual void connectToServer(std::string ip);
	virtual void connectToServer(std::string ip, short int port);
	virtual void connectToServer(int ip, short int port);

	virtual void handleInteraction(Interaction *action);
	virtual void update();
	virtual void close();
	virtual ~Connection();
};

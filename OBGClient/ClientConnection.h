#pragma once
#include <PhysicsUpdateEventBroadcaster.h>
#include <Connection.h>
#include <InteractionListener.h>
#include "MessageEventBroadcaster.h"
#include "MessageListener.h"

class ClientConnection :
	public Connection,
	public PhysicsUpdateEventBroadcaster,
	public MessageEventBroadcaster,
	public InteractionListener,
	public MessageListener
{
protected:
	void processData(const SerialData &data);
	virtual void handleFatalError();

public:
	ClientConnection(std::string ip, short port);

	virtual void setUsername(const std::string &username);
	virtual void handleInteraction(Interaction *action);
	virtual void handleMessage(const std::string &message);

	virtual ~ClientConnection();
};

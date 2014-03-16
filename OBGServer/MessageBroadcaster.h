#pragma once

#include "ServerMessageListener.h"

class MessageBroadcaster :
	public ServerMessageListener
{
public:
	virtual void handleMessage(const std::string &msg, ServerConnection *player);
};

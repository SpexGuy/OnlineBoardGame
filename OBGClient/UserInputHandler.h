#pragma once
#include <InteractionEventBroadcaster.h>
#include "MessageEventBroadcaster.h"

class UserInputHandler :
	public MessageEventBroadcaster,
	public InteractionEventBroadcaster
{
public:
	virtual void update();
};

#pragma once
#include "InteractionListener.h"
#include "PhysicsUpdateListener.h"
#include "PhysicsUpdateEventBroadcaster.h"

class Entity;

class EntityManager :
	public InteractionListener,
	public PhysicsUpdateListener,
	public PhysicsUpdateEventBroadcaster
{
public:
	EntityManager();

	virtual void addEntity(Entity *e);

	
	virtual void handleInteraction(Interaction *action);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);

};

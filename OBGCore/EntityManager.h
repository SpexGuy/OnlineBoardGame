#pragma once
#include "InteractionListener.h"
#include "PhysicsUpdateListener.h"
#include "PhysicsUpdateEventBroadcaster.h"
#include "btBulletDynamicsCommon.h"
#include <vector>
#include <LinearMath\btVector3.h>
#include "Asset.h"
#include "Interaction.h"
#include "PhysicsUpdate.h"
#include <map>

using std::vector;
using std::map;
using std::pair;

class Entity;

class EntityManager :
	public InteractionListener,
	public PhysicsUpdateListener,
	public PhysicsUpdateEventBroadcaster
{
public:
	EntityManager();
	~EntityManager();

	virtual void addEntity(Entity *e);

	
	virtual void handleInteraction(Interaction *action);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);

	virtual void registerPhysicsUpdateListener(PhysicsUpdateListener *listener);
	virtual void update();
	virtual void clear();
	virtual Entity* getEntityByIndex(int);
	virtual Entity* getIntersectingEntity(const btVector3&, const btVector3&, Interaction*);
	virtual void createPhysicsUpdates();

private:
	map<int, Entity*> entities;
	btDiscreteDynamicsWorld* world;
	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
};
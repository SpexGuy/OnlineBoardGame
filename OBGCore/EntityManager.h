#pragma once
#include <map>
#include <time.h>
#include "btBulletDynamicsCommon.h"
#include "InteractionListener.h"
#include "PhysicsUpdateListener.h"
#include "PhysicsUpdateEventBroadcaster.h"

class Entity;
class Interaction;
class PhysicsUpdate;

class EntityManager :
	public InteractionListener,
	public PhysicsUpdateListener,
	public PhysicsUpdateEventBroadcaster
{
protected:
	std::map<int, Entity*> entities;
	btDiscreteDynamicsWorld* world;
	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
	clock_t lastTime;

public:
	EntityManager();
	virtual ~EntityManager();

	virtual void addEntity(Entity *e);

	virtual void handleInteraction(Interaction *action);
	virtual void handlePhysicsUpdate(PhysicsUpdate *update);

	virtual void start();
	virtual void update();
	virtual void clear();
	virtual Entity* getEntityById(int id);
	virtual std::map<int, Entity *> getEntities() { return entities; }
	virtual Entity* getIntersectingEntity(const btVector3& from, const btVector3& to);
	virtual void createPhysicsUpdates();
};
#pragma once
#include "InteractionListener.h"
#include "PhysicsUpdateListener.h"
#include "PhysicsUpdateEventBroadcaster.h"
#include "btBulletDynamicsCommon.h"
#include <vector>
#include <LinearMath\btVector3.h>
#include <map>
#include <time.h>

using std::vector;
using std::map;
using std::pair;

class Entity;
class Interaction;
class PhysicsUpdate;

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
	virtual Entity* getEntityById(int id);
	virtual Entity* getIntersectingEntity(const btVector3& from, const btVector3& to, Interaction* interaction);
	virtual void createPhysicsUpdates();

private:
	map<int, Entity*> entities;
	btDiscreteDynamicsWorld* world;
	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
	time_t lastTime;
};
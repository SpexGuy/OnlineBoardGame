#pragma once
#include <btBulletDynamicsCommon.h>

class Asset;
class ShakeStrategy;

class Entity :
	public btMotionState
{
private:
	Entity();
protected:
	int id;
	bool hidden;
	btRigidBody *physicsBody;
	btTransform transform;
	Asset *type;
	ShakeStrategy *shakeStrategy;

public:
	Entity(Asset *type, int id, const btTransform &transform, ShakeStrategy *strategy);
	virtual void shake();
	virtual void hide();
	virtual void show();
	virtual ~Entity();

	inline int getId() { return id; }
	virtual void setWorldTransform(const btTransform &t) { transform = t; }
	virtual void getWorldTransform(btTransform &t) const { t = transform; }
	inline void setRigidBody(btRigidBody *body) { physicsBody = body; }
	inline btRigidBody *getPhysicsBody() { return physicsBody; }
	inline bool getHidden() { return hidden; }
	inline Asset *getType() { return type; }
};

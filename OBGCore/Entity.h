#pragma once
#include <btBulletDynamicsCommon.h>

class Asset;

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

public:
	Entity(Asset *type, int id, const btTransform &transform);
	virtual void shake();
	virtual void hide();
	virtual void show();
	virtual ~Entity();

	inline int getId() { return id; }
	virtual void setWorldTransform(const btTransform &t) { transform = t; }
	virtual void getWorldTransform(btTransform &t) const { t = transform; }
	inline void setRigidBody(btRigidBody *body) { physicsBody = body; }
	inline btRigidBody *getPhysicsBody() { return physicsBody; }
	inline Asset *getType() { return type; }
};

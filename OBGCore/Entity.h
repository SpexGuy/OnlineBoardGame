#pragma once
#include <btBulletDynamicsCommon.h>

class Asset;

class Entity {
private:
	Entity();
protected:
	int id;
	bool hidden;
	btRigidBody *physicsBody;
	Asset *type;

public:
	inline int getId() {return id;}
	inline btRigidBody *getPhysicsBody() {return physicsBody;}
	inline Asset *getType() {return type;}
	Entity(Asset *type, int id, btRigidBody *physicsBody);
	virtual void shake();
	virtual void hide();
	virtual void show();
	virtual ~Entity();
};

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
	Entity(Asset *type, const btVector3 &position) :
		type(type), id(0), hidden(false) {}
	virtual void shake();
	virtual void hide();
	virtual void show();
	virtual ~Entity();
};

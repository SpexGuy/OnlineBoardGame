#pragma once
#include <btBulletDynamicsCommon.h>

class Asset;

class Entity {
private:
	Entity();
protected:
	Asset *type;
	int id;
	bool hidden;

public:
	Entity(Asset *type, const btVector3 &position) :
		type(type), id(0), hidden(false) {}
	virtual void shake();
	virtual void hide();
	virtual void show();
	virtual ~Entity();
};

#include "Entity.h"
#include "Asset.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

Entity::Entity() {

}

Entity::Entity(Asset *type, int id, const btTransform &transform) :
	type(type),
	id(id),
	transform(transform),
	hidden(false)
{}

void Entity::shake() {
	btTransform trans = this->getPhysicsBody()->getWorldTransform();
	float randY, randP, randR;
	randY = rand()/(float)RAND_MAX;
	randP = rand()/(float)RAND_MAX;
	randR = rand()/(float)RAND_MAX;

	randY *= 2 * M_PI;
	randP *= 2 * M_PI;
	randR *= 2 * M_PI;

	trans.setRotation(btQuaternion(randY, randP, randR));
	this->getPhysicsBody()->setWorldTransform(trans);
}

void Entity::hide() {
	hidden = true;
}

void Entity::show() {
	hidden = false;
}

Entity::~Entity() {

}

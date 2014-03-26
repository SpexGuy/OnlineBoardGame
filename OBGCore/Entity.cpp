#include "Entity.h"
#include "Asset.h"

Entity::Entity() {

}

Entity::Entity(Asset *type, int id, btRigidBody *physicsBody) {
	this->type = type;
	this->id = id;
	this->hidden = false;
	this->physicsBody = physicsBody;
}

void Entity::shake() {
	//TODO:[JK] shake physics
}

void Entity::hide() {
	hidden = true;
}

void Entity::show() {
	hidden = false;
}

Entity::~Entity() {

}

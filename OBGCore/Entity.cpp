#include "Entity.h"
#include "Asset.h"

Entity::Entity() {

}

Entity::Entity(Asset *type, int id, const btTransform &transform) :
	type(type),
	id(id),
	transform(transform),
	hidden(false)
{}

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

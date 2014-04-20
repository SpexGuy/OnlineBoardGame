#include <iostream>
#include "Asset.h"
#include "Entity.h"
#include "ShakeStrategy.h"

Entity::Entity(Asset *type, int id, const btTransform &transform, ShakeStrategy *shakeStrategy) :
	type(type),
	id(id),
	transform(transform),
	hidden(false),
	shakeStrategy(shakeStrategy)
{}

void Entity::shake() {
	shakeStrategy->shake(this);
}

void Entity::hide() {
	hidden = true;
}

void Entity::show() {
	hidden = false;
}

Entity::~Entity() {

}

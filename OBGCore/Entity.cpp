#include "Entity.h"
#include "Asset.h"

Entity::Entity() {
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

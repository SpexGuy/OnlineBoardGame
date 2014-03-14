#include "Asset.h"
#include "Entity.h"

Entity *Asset::createEntity(const btVector3 &position) {
	return new Entity(this, position);
}

Asset::~Asset() {

}
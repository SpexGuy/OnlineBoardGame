#include <glm/glm.hpp>
#include "GraphicsAsset.h"
#include "GraphicsEntity.h"

using namespace glm;

GraphicsEntity::GraphicsEntity(GraphicsAsset *asset,
							   const btVector3 &position)
		: Entity(asset, position)
{}

void GraphicsEntity::render() {
	//this cast is safe because type never changes and
	//it must be a GraphicsAsset in the constructor
	((GraphicsAsset *)type)->draw(mat4(1.0f));
}

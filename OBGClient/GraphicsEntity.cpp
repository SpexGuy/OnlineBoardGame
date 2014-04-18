#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsAsset.h"
#include "GraphicsEntity.h"

using namespace glm;

GraphicsEntity::GraphicsEntity(GraphicsAsset *asset, int id,
							   const btTransform &t) :
	Entity(asset, id, t)
{}

void GraphicsEntity::render() {
	if (!hidden) {
		//this cast is safe because type never changes and
		//it must be a GraphicsAsset in the constructor
		((GraphicsAsset *)type)->draw(world);
	}
}

void GraphicsEntity::setWorldTransform(const btTransform &transform) {
	Entity::setWorldTransform(transform);
	transform.getOpenGLMatrix(value_ptr(world));
}

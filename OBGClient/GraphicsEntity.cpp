#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsAsset.h"
#include "GraphicsEntity.h"

using namespace glm;

GraphicsEntity::GraphicsEntity(GraphicsAsset *asset, int id,
							   btRigidBody *physicsBody) :
	Entity(asset, id, physicsBody)
{}

void GraphicsEntity::render() {
	if (!hidden) {
		mat4 world;
		btTransform transform;
		physicsBody->getMotionState()->getWorldTransform(transform);
		transform.getOpenGLMatrix(value_ptr(world));
		//this cast is safe because type never changes and
		//it must be a GraphicsAsset in the constructor
		((GraphicsAsset *)type)->draw(world);
	}
}

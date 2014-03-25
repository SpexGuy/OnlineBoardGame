#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsAsset.h"
#include "GraphicsEntity.h"

using namespace glm;

GraphicsEntity::GraphicsEntity(GraphicsAsset *asset,
							   const btVector3 &position)
		: Entity(asset, position)
{}

void GraphicsEntity::render() {
	if (!hidden) {
		//this cast is safe because type never changes and
		//it must be a GraphicsAsset in the constructor
		((GraphicsAsset *)type)->draw(rotate(mat4(1.0f), float(glutGet(GLUT_ELAPSED_TIME))*0.025f, vec3(0,1,0)));
		//TODO:[MW] stop entities from spinning ALL THE TIME
	}
}

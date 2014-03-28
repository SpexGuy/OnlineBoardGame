#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <GraphicsContext.h>
#include "CollisionMeshRenderer.h"

using namespace std;
using namespace glm;

void CollisionMeshRenderer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(GraphicsContext::inst()->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(GraphicsContext::inst()->getView()));
	glBegin(GL_LINES);
	glColor3f(color.getX(), color.getY(), color.getZ());
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void CollisionMeshRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
								btScalar distance, int lifeTime, const btVector3& color)
{

}

void CollisionMeshRenderer::reportErrorWarning(const char* warningString) {
	std::cout << "[debugRenderer]:" << warningString << std::endl;
}

void CollisionMeshRenderer::draw3dText(const btVector3& location,const char* textString) {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(GraphicsContext::inst()->getProjection()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(GraphicsContext::inst()->getView()));
	glTranslatef(location.getX(), location.getY(), location.getZ());
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char *)textString);
}

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GraphicsContext.h>
#include "GraphicsManager.h"

using namespace std;
using namespace glm;

GraphicsManager::GraphicsManager(int argc, char *argv[]) {
	OBGGraphicsInit(argc, argv);
}

void GraphicsManager::start() {
	OBGGraphicsCreateContext();
	GraphicsContext::inst()->setView(
		glm::lookAt(vec3(2.0f, 2.0f, 2.0f),
					vec3(0.0f, 0.0f, 0.0f),
					vec3(0.0f, 1.0f, 0.0f)));
}

void GraphicsManager::display() {
	GraphicsContext::inst()->setupFrame();
	for (unsigned int c = 0; c < renderables.size(); c++) {
		renderables[c]->render();
	}
	glutSwapBuffers();
}

void GraphicsManager::reshape(int x, int y) {
	GraphicsContext::inst()->setSize(x, y);
	GraphicsContext::inst()->setProjection(
		glm::perspective(45.0f, float(x)/y, 0.1f, 10.0f));

}

void GraphicsManager::addRenderable(Renderable *r) {
	renderables.push_back(r);
}


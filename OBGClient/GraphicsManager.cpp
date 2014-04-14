#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GraphicsContext.h>
#include "GameManager.h"
#include "GraphicsManager.h"
#include "MousePointer.h"
#include "UserInputHandler.h"

#define ZOOM_SCALE 0.4

using namespace std;
using namespace glm;

GraphicsManager::GraphicsManager(int argc, char *argv[]) {
	OBGGraphicsInit(argc, argv);
	zoomed = false;
}

void GraphicsManager::start() {
	OBGGraphicsCreateContext();
	GraphicsContext::inst()->setView(
		glm::lookAt(vec3(0.0f, 40.0f, 20.0f),
					vec3(0.0f, 0.0f, 0.0f),
					vec3(0.0f, 1.0f, 0.0f)));
}

void GraphicsManager::display() {
	if (zoomed) {
		vec3 mouse = GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos();
		GraphicsContext::inst()->setView(
			glm::lookAt(vec3(mouse.x, 40*ZOOM_SCALE, mouse.z + 20*ZOOM_SCALE),
						vec3(mouse.x,  0.0f, mouse.z),
						vec3(0.0f, 1.0f, 0.0f)));
	}
	GraphicsContext::inst()->setupFrame();
	for (unsigned int c = 0; c < renderables.size(); c++) {
		renderables[c]->render();
	}
	glutSwapBuffers();
}

void GraphicsManager::toggleZoom() {
	zoomed = !zoomed;
	if (!zoomed) {
		GraphicsContext::inst()->setView(
			glm::lookAt(vec3(0.0f, 40.0f, 20.0f),
						vec3(0.0f, 0.0f, 0.0f),
						vec3(0.0f, 1.0f, 0.0f)));
	}
}

void GraphicsManager::reshape(int x, int y) {
	GraphicsContext::inst()->setSize(x, y);
	GraphicsContext::inst()->setProjection(
		glm::perspective(45.0f, float(x)/y, 0.1f, 50.0f));
}

void GraphicsManager::addRenderable(Renderable *r) {
	renderables.push_back(r);
}


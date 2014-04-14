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

GraphicsManager::GraphicsManager(int argc, char *argv[]) :
	cameraPos(0, 250, vec3(0,40,20))
{
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
	vec3 pos = cameraPos.get();
	GraphicsContext::inst()->setView(
		glm::lookAt(pos, pos - vec3(0,40,20), vec3(0,1,0)));
	GraphicsContext::inst()->setupFrame();
	for (unsigned int c = 0; c < renderables.size(); c++) {
		renderables[c]->render();
	}
	glutSwapBuffers();
}

void GraphicsManager::update(int time) {
	if (zoomed) {
		vec3 mouse = GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos();
		cameraPos.setTarget(vec3(mouse.x, 40*ZOOM_SCALE, mouse.z + 20*ZOOM_SCALE));
	}
	cameraPos.update(time);
}

void GraphicsManager::toggleZoom() {
	zoomed = !zoomed;
	if (zoomed) {
		vec3 mouse = GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos();
		cameraPos.set(vec3(mouse.x, 40*ZOOM_SCALE, mouse.z + 20*ZOOM_SCALE));
	} else {
		cameraPos.set(vec3(0,40,20));
	}
}

void GraphicsManager::reshape(int x, int y) {
	GraphicsContext::inst()->setSize(x, y);
	GraphicsContext::inst()->setProjection(
		glm::perspective(45.0f, float(x)/y, 0.1f, 80.0f));
}

void GraphicsManager::addRenderable(Renderable *r) {
	renderables.push_back(r);
}


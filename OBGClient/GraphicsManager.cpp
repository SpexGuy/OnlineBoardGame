#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GraphicsContext.h>
#include "Constants.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "MousePointer.h"
#include "UserInputHandler.h"

#define ZOOM_SCALE 0.4
#define CAM_Y 2*BOARD_SIZE
#define CAM_Z BOARD_SIZE
#define CAM_POS vec3(0,CAM_Y,CAM_Z)

using namespace std;
using namespace glm;

GraphicsManager::GraphicsManager(int argc, char *argv[]) :
	cameraPos(0, 250, CAM_POS)
{
	OBGGraphicsInit(argc, argv);
	zoomed = false;
}

void GraphicsManager::start() {
	OBGGraphicsCreateContext();
}

void GraphicsManager::display() {
	ivec2 size = GraphicsContext::inst()->getWindowSize();
	GraphicsContext::inst()->setProjection(
		glm::perspective(45.0f, float(size.x)/size.y, 0.1f, 2.0f*CAM_Y));
	vec3 pos = cameraPos.get();
	GraphicsContext::inst()->setView(
		glm::lookAt(pos, pos - CAM_POS, vec3(0,1,0)));
	GraphicsContext::inst()->setupFrame();
	for (unsigned int c = 0; c < renderables.size(); c++) {
		renderables[c]->render();
	}
	GraphicsContext::inst()->swapBuffers();
}

void GraphicsManager::update(int time) {
	if (zoomed) {
		vec3 mouse = GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos();
		cameraPos.setTarget(vec3(mouse.x, CAM_Y*ZOOM_SCALE, mouse.z + CAM_Z*ZOOM_SCALE));
	}
	cameraPos.update(time);
}

void GraphicsManager::toggleZoom() {
	zoomed = !zoomed;
	if (zoomed) {
		vec3 mouse = GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos();
		cameraPos.set(vec3(mouse.x, CAM_Y*ZOOM_SCALE, mouse.z + CAM_Z*ZOOM_SCALE));
	} else {
		cameraPos.set(CAM_POS);
	}
}

void GraphicsManager::addRenderable(Renderable *r) {
	renderables.push_back(r);
}


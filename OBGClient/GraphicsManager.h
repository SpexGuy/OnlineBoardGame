#pragma once
#include <vector>
#include <Renderable.h>
#include "SmoothValue.h"

class GraphicsManager {
protected:
	std::vector<Renderable *> renderables;
	SmoothValue<glm::vec3> cameraPos;
	bool zoomed;

public:
	GraphicsManager(int argc, char *argv[]);
	virtual void start();
	virtual void update(int time);
	virtual void display();
	virtual void toggleZoom();
	virtual void addRenderable(Renderable *r);
};

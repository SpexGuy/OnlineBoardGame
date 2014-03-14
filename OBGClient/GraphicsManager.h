#pragma once
#include <vector>
#include <Renderable.h>

class GraphicsManager {
protected:
	std::vector<Renderable *> renderables;

public:
	GraphicsManager(int argc, char *argv[]);
	virtual void display();
	virtual void addRenderable(Renderable *r);
};

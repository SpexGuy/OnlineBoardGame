#pragma once
#include <vector>
#include <Renderable.h>

class GraphicsManager {
protected:
	std::vector<Renderable *> renderables;
	bool zoomed;

public:
	GraphicsManager(int argc, char *argv[]);
	virtual void start();
	virtual void display();
	virtual void toggleZoom();
	virtual void reshape(int x, int y);
	virtual void addRenderable(Renderable *r);
};

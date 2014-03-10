#pragma once
#include <vector>

class Renderable;

class GraphicsManager {
protected:
	std::vector<Renderable> renderables;

public:
	virtual void update();
	virtual void addRenderable(Renderable *r);
	virtual void 
};

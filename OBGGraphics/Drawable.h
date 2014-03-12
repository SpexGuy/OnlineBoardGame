#pragma once
#include <glm/glm.hpp>

class Drawable {
public:
	virtual void draw(const glm::mat4 &world) = 0;
	virtual ~Drawable() {}
};

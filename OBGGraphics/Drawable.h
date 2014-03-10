#pragma once

class Drawable {
public:
	virtual void draw(/*TODO:mat4*/) = 0;
	virtual ~Drawable() {}
};

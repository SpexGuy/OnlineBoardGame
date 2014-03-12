#pragma once
#include "Drawable.h"

class GraphicsMesh :
	public Drawable
{
	virtual void draw(const glm::mat4 &world);
	virtual ~GraphicsMesh();
};

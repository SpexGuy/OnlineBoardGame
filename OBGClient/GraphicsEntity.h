#pragma once
#include <Entity.h>
#include <Renderable.h>

class GraphicsEntity :
	public Entity,
	public Renderable
{
	virtual void render();
};

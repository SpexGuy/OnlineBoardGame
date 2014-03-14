#pragma once
#include <Entity.h>
#include <Renderable.h>

class GraphicsAsset;

class GraphicsEntity :
	public Entity,
	public Renderable
{
private:
	GraphicsEntity();
public:
	GraphicsEntity(GraphicsAsset *asset, const btVector3 &position);
	virtual void render();
	virtual ~GraphicsEntity() {}
};

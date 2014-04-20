#pragma once
#include <Entity.h>
#include <Renderable.h>

class GraphicsAsset;
class ShakeStrategy;

class GraphicsEntity :
	public Entity,
	public Renderable
{
private:
	GraphicsEntity();
	glm::mat4 world;

public:
	GraphicsEntity(GraphicsAsset *asset, int id, const btTransform &t, ShakeStrategy *shake);
	virtual void render();
	virtual void setWorldTransform(const btTransform &transform);
	virtual ~GraphicsEntity() {}
};

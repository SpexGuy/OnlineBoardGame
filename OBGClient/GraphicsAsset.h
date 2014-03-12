#pragma once
#include <Asset.h>
#include <Drawable.h>
#include "GraphicsEntity.h"

class GraphicsMesh;
class Material;

class GraphicsAsset :
	public Asset,
	public Drawable
{
protected:
	GraphicsMesh *mesh;
	Material *material;
public:
	virtual void draw(const glm::mat4 &world);
	virtual GraphicsEntity *createEntity(btVector3 &position);
	virtual ~GraphicsAsset();
};

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
	GraphicsAsset(const std::string &name, const std::string &group,
				  float mass, const btVector3 &position,
				  btTriangleMesh *collider,
				  GraphicsMesh *mesh, Material *material)
	 :	Asset(name, group, mass, position, collider),
		mesh(mesh),
		material(material)
	{}

	virtual void draw(const glm::mat4 &world);
	virtual GraphicsEntity *createEntity(const btVector3 &position);
	virtual ~GraphicsAsset() {}
};

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
				  float mass, const btVector3 &centerMass,
				  const btTransform &orientation,
				  CollisionShapeInflater *collider,
				  GraphicsMesh *mesh, Material *material, ShakeStrategy *shake)
	 :	Asset(name, group, mass, centerMass, orientation, collider, shake),
		mesh(mesh),
		material(material)
	{}

	virtual void draw(const glm::mat4 &world);
	virtual GraphicsEntity *createEntity(const btTransform &position, int id);
	virtual ~GraphicsAsset() {}
};

#include <GraphicsMesh.h>
#include <ILTexture.h>
#include <Material.h>
#include <Texture.h>
#include <TextureGroup.h>
#include <TextureMaterial.h>
#include "ClientConnection.h"
#include "GameManager.h"
#include "GraphicsAsset.h"
#include "GraphicsAssetPack.h"
#include "PointerShadowShader.h"

//there's probably a better place for these
#define CHANNEL_COLOR 0
#define CHANNEL_SPECULAR 1
#define CHANNEL_NORMAL 2

using namespace std;
using Json::Value;
using Json::ValueType;


GraphicsAssetPack::GraphicsAssetPack(const Value &root) :
		AssetPack(root)
{
	texShader = new PointerShadowShader();
	texShader->compileShader("PointerShadowShader.vert");
	texShader->compileShader("PointerShadowShader.frag");
	texShader->link();
}

void GraphicsAssetPack::downloadFile(const string &file) {
	GameManager::inst()->getConnection()->downloadFile(file);
}

GraphicsAsset *GraphicsAssetPack::makeAsset(const string &name) {
	Value asset = assetRoot[name];
	assert(asset.isObject());
	Value group = asset["Group"];
	Value stackType = asset["StackType"];
	Value shakeType = asset["ShakeType"];
	Value meshName = asset["Mesh"];
	Value colorTex = asset["ColorTex"];
	Value specTex = asset["SpecularTex"];
	Value normalTex = asset["NormalTex"];
	Value mass = asset["Mass"];
	Value massCenterX = asset["CoMx"];
	Value massCenterY = asset["CoMy"];
	Value massCenterZ = asset["CoMz"];
	Value collider = asset["Collider"];
	assert(group.isConvertibleTo(ValueType::stringValue));
	assert(stackType.isNull());
	assert(shakeType.isNull());
	assert(meshName.isString());
	assert(colorTex.isString());
	assert(specTex.isString());
	assert(normalTex.isString());
	assert(mass.isConvertibleTo(ValueType::realValue));
	assert(massCenterX.isConvertibleTo(ValueType::realValue));
	assert(massCenterY.isConvertibleTo(ValueType::realValue));
	assert(massCenterZ.isConvertibleTo(ValueType::realValue));

	CollisionShapeInflater *shape;
	btTransform transform;
	bool success = parseCollider(collider, &transform, &shape);
	assert(success);

	Texture *tex = (new TextureGroup())
		->addTexture(new ILTexture(getImage(colorTex.asString()), CHANNEL_COLOR))
		->addTexture(new ILTexture(getImage(specTex.asString()), CHANNEL_SPECULAR))
		->addTexture(new ILTexture(getImage(normalTex.asString()), CHANNEL_NORMAL));

	GraphicsMesh *mesh = getMesh(meshName.asString());

	TextureMaterial *material = new TextureMaterial(tex, texShader);

	return new GraphicsAsset(name, group.asString(), mass.asDouble(),
		btVector3(massCenterX.asDouble(), massCenterY.asDouble(), massCenterZ.asDouble()),
		transform, shape, mesh, material);
}

ifstream *GraphicsAssetPack::getFile(const string &filename) {
	ifstream *stream = new ifstream(filename, ios::in);
	if (!*stream) {
		delete stream;
		downloadFile(filename);
		stream = new ifstream(filename, ios::in);
		if (!*stream) {
			delete stream;
			return NULL;
		}
	}
	return stream;
}

GraphicsMesh *GraphicsAssetPack::getMesh(const string &filename) {
	auto pos = meshes.find(filename);
	GraphicsMesh *mesh;
	if (pos == meshes.end()) {
		//load the mesh
		ifstream *file = getFile(filename);
		if (file == NULL) {
			cout << "Could not load file " << filename << endl;
			return NULL;
		}
		mesh = GraphicsMesh::loadMesh(file);
		file->close();
		delete file;
		if (mesh == NULL) {
			cout << "Could not load mesh from " << filename << endl;
			return NULL;
		}
		meshes[filename] = mesh;
	} else {
		mesh = pos->second;
	}
	return mesh;
}

ILContainer *GraphicsAssetPack::getImage(const string &filename) {
	auto pos = images.find(filename);
	if (pos == images.end()) {
		ILContainer *image = new ILContainer(filename.c_str());
		images[filename] = image;
		return image;
	} else {
		return pos->second;
	}
}

GraphicsAssetPack::~GraphicsAssetPack() {
	//free all allocated resources
	for (auto iter : meshes) {
		delete iter.second;
	}
	meshes.clear();
}

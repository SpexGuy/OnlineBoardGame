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


GraphicsAssetPack::GraphicsAssetPack() {}

bool GraphicsAssetPack::initialize(const Value &root) {
	if (!AssetPack::initialize(root))
		return false;
	texShader = new PointerShadowShader();
	texShader->compileShader("PointerShadowShader.vert");
	texShader->compileShader("PointerShadowShader.frag");
	texShader->link();
	return true;
}

void GraphicsAssetPack::downloadFile(const string &file) {
	GameManager::inst()->getConnection()->downloadFile(file);
}

GraphicsAsset *GraphicsAssetPack::makeAsset(const string &name) {
	auto loc = assetValues.find(name);
	if (loc == assetValues.end())
		return NULL;
	Value asset = loc->second;
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
	if (!group.isConvertibleTo(ValueType::stringValue)) return NULL;
	if (!meshName.isString()) return NULL;
	if (!colorTex.isString()) return NULL;
	if (!specTex.isString()) return NULL;
	if (!normalTex.isString()) return NULL;
	if (!mass.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterX.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterY.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterZ.isConvertibleTo(ValueType::realValue)) return NULL;

	CollisionShapeInflater *shape;
	btTransform transform;
	bool success = parseCollider(collider, &transform, &shape);
	if (!success) return NULL;

	ShakeStrategy *shake;
	success = getShakeStrategy(shakeType, &shake);
	if (!success) return NULL;

	TextureGroup *tex = new TextureGroup();
	ILContainer *color = getImage(colorTex.asString());
	if (!color) return NULL;
	tex->addTexture(new ILTexture(color, CHANNEL_COLOR));
	if (*specTex.asCString() != '\0') {
		ILContainer *spec = getImage(specTex.asString());
		if (!spec) return NULL;
		tex->addTexture(new ILTexture(spec, CHANNEL_SPECULAR));
	}
	if (*normalTex.asCString() != '\0') {
		ILContainer *norm = getImage(normalTex.asString());
		if (!norm) return NULL;
		tex->addTexture(new ILTexture(norm, CHANNEL_NORMAL));
	}

	GraphicsMesh *mesh = getMesh(meshName.asString());
	if (!mesh) return NULL;

	TextureMaterial *material = new TextureMaterial(tex, texShader);
	if (!material) return NULL;

	return new GraphicsAsset(name, group.asString(), btScalar(mass.asDouble()),
		btVector3(btScalar(massCenterX.asDouble()),
				  btScalar(massCenterY.asDouble()),
				  btScalar(massCenterZ.asDouble())),
		transform, shape, mesh, material, shake);
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
		if (!image->initialize()) {
			delete image;
			image = NULL;
		}
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

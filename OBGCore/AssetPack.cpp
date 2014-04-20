#include <assert.h>
#include <fstream>
#include <iostream>
#include "Asset.h"
#include "AssetPack.h"
#include "CollisionMeshLoader.h"
#include "CollisionShapeInflater.h"
#include "Entity.h"
#include "json.h"

using namespace std;
using Json::Value;
using Json::ValueType;

btQuaternion parseQuaternion(const Value &val) {
	assert(val.isArray() && val.size() == 4);
	assert(val[0].isConvertibleTo(ValueType::realValue));
	assert(val[1].isConvertibleTo(ValueType::realValue));
	assert(val[2].isConvertibleTo(ValueType::realValue));
	assert(val[3].isConvertibleTo(ValueType::realValue));
	return btQuaternion(val[0].asDouble(), val[1].asDouble(),
						val[2].asDouble(), val[3].asDouble());
}

btVector3 parseVector3(const Value &val) {
	assert(val.isArray() && val.size() == 3);
	assert(val[0].isConvertibleTo(ValueType::realValue));
	assert(val[1].isConvertibleTo(ValueType::realValue));
	assert(val[2].isConvertibleTo(ValueType::realValue));
	return btVector3(val[0].asDouble(), val[1].asDouble(),
					 val[2].asDouble());
}

AssetPack::AssetPack(const Json::Value &root) {
	assert(root.isObject());
	Value version = root["Version"];
	Value gameName = root["GameName"];
	Value defaultSave = root["EntityLocationsFile"];
	Value assetRoot = root["Assets"];
	
	assert(version.isConvertibleTo(ValueType::realValue));
	assert(gameName.isString());
	assert(defaultSave.isString());
	assert(assetRoot.isArray());

	this->version = version.asDouble();
	this->gameName = gameName.asString();
	this->defaultSaveFile = defaultSave.asString();
	this->assetRoot = assetRoot;
	Value assetName;
	for (Value asset : assetRoot) {
		if (!asset.isObject()) continue;
		assetName = asset["Name"];
		if (!assetName.isString()) continue;
		assert(assetValues.find(assetName.asString()) == assetValues.end());
		assetValues[assetName.asString()] = asset;
	}
}

Asset *AssetPack::makeAsset(const string &name) {
	auto loc = assetValues.find(name);
	if (loc == assetValues.end())
		return NULL;
	Value asset = loc->second;
	assert(asset.isObject());

	Value group = asset["Group"];
	Value stackType = asset["StackType"];
	Value shakeType = asset["ShakeType"];
	Value mass = asset["Mass"];
	Value massCenterX = asset["CoMx"];
	Value massCenterY = asset["CoMy"];
	Value massCenterZ = asset["CoMz"];
	Value collider = asset["Collider"];
	assert(group.isConvertibleTo(ValueType::stringValue));
	assert(stackType.isNull());
	assert(shakeType.isNull());
	assert(mass.isConvertibleTo(ValueType::realValue));
	assert(massCenterX.isConvertibleTo(ValueType::realValue));
	assert(massCenterY.isConvertibleTo(ValueType::realValue));
	assert(massCenterZ.isConvertibleTo(ValueType::realValue));

	CollisionShapeInflater *shape;
	btTransform transform;
	bool success = parseCollider(collider, &transform, &shape);
	assert(success);

	return new Asset(
		name,
		group.asString(),
		mass.asDouble(),
		btVector3(massCenterX.asDouble(),
				  massCenterY.asDouble(),
				  massCenterZ.asDouble()),
		transform, shape);
}

Asset *AssetPack::getAsset(const string &name) {
	auto location = assets.find(name);
	if (location == assets.end()) {
		Asset *newAsset = makeAsset(name);
		if (newAsset == NULL) {
			cout << "Could not load asset: " << name << endl;
			//DO NOT PREVENT CACHE - Fail is deterministic.
		}
		assets[name] = newAsset;
		return newAsset;
	} else {
		return location->second;
	}
}

bool AssetPack::parseCollider(const Value &collider, btTransform *retTransform, CollisionShapeInflater **retShape) {
	assert(collider.isObject());
	Value type = collider["Type"];
	Value position = collider["Position"];
	Value rotation = collider["Rotation"];
	assert(type.isString());
	*retTransform = btTransform(parseQuaternion(rotation),parseVector3(position));


	if (type.asString() == string("Sphere")) {
		Value radius = collider["Radius"];
		assert(radius.isConvertibleTo(ValueType::realValue));
		*retShape = new SphereInflater(radius.asDouble());
		return true;
	} else if (type.asString() == string("Box")) {
		Value halfExtents = collider["HalfExtents"];
		btVector3 vec = parseVector3(halfExtents);
		*retShape = new BoxInflater(vec);
		return true;
	} else if (type.asString() == string("Cylinder")) {
		Value halfExtents = collider["HalfExtents"];
		btVector3 vec = parseVector3(halfExtents);
		*retShape = new CylinderInflater(vec);
		return true;
	} else if (type.asString() == string("Capsule")) {
		Value radius = collider["Radius"];
		Value distance = collider["Distance"];
		assert(radius.isConvertibleTo(ValueType::realValue));
		assert(distance.isConvertibleTo(ValueType::realValue));
		*retShape = new CapsuleInflater(radius.asDouble(), distance.asDouble());
		return true;
	} else if (type.asString() == string("Cone")) {
		Value radius = collider["Radius"];
		Value height = collider["Height"];
		assert(radius.isConvertibleTo(ValueType::realValue));
		assert(height.isConvertibleTo(ValueType::realValue));
		*retShape = new ConeInflater(radius.asDouble(), height.asDouble());
		return true;
	} else if (type.asString() == string("MultiSphere")) {
		Value radiiVal = collider["Radii"];
		Value positionsVal = collider["Positions"];
		assert(radiiVal.isArray());
		assert(positionsVal.isArray());
		assert(radiiVal.size() > 0);
		assert(radiiVal.size() == positionsVal.size());
		int num = radiiVal.size();
		btVector3 *positions = new btVector3[num];
		btScalar *radii = new btScalar[num];
		for (int c = 0; c < num; c++) {
			assert(radiiVal[c].isConvertibleTo(ValueType::realValue));
			positions[c] = parseVector3(positionsVal[c]);
			radii[c] = radiiVal[c].asDouble();
		}
		*retShape = new MultiSphereInflater(positions, radii, num);
		//positions and radii are deleted by the inflater
		return true;
	} else if (type.asString() == string("ConvexHull")) {
		Value file = collider["File"];
		assert(file.isString());
		CollisionShapeInflater *shape = getCollider(file.asString());
		*retShape = shape;
		return shape != NULL;
	} else if (type.asString() == string("Empty")) {
		*retShape = new EmptyInflater();
		return true;
	} else if (type.asString() == string("Compound")) {
		Value colliders = collider["Colliders"];
		assert(colliders.isArray());
		assert(colliders.size() > 0);
		CompoundInflater *shape = new CompoundInflater();
		btTransform subTrans;
		CollisionShapeInflater *subShape;
		int numChildren = 0;
		for (Value subColl : colliders) {
			if (parseCollider(subColl, &subTrans, &subShape)) {
				shape->addInflater(subTrans, subShape);
				numChildren++;
			} else {
				cout << "Could not parse child of compound collider. Ignoring." << endl;
			}
		}
		*retShape = shape;
		return numChildren > 0;
	} else {
		cout << "Unknown Collider Type: " << type.asString() << endl;
		assert(false);
		return false;
	}
	//this should be unreachable, but just in case...
	assert(false);
	return false;
}

CollisionShapeInflater *AssetPack::getCollider(const string &name) {
	auto pos = colliders.find(name);
	if (pos == colliders.end()) {
		ifstream file(name);
		if (!file) {
			colliders[name] = NULL;
			return NULL;
		}
		vector<btVector3> *mesh = loadCollisionPoints(&file);
		colliders[name] = mesh;
		return new ConvexHullInflater(*mesh);
	} else {
		return new ConvexHullInflater(*pos->second);
	}
}

vector<Entity *> AssetPack::loadGame() {
	return loadGame(defaultSaveFile);
}

vector<Entity *> AssetPack::loadGame(const string &saveFile) {
	int id = 1;
	vector<Entity *> entities;
	Json::Reader reader;
	Value saveRoot;
	ifstream file(saveFile, ios::in);
	if (!file) {
		cout << "Could not open entity file: " << saveFile << endl;
		return entities;
	}
	if (!reader.parse(file, saveRoot)) {
		cout << "Could not parse entity file: " << saveFile << endl;
		file.close();
		return entities;
	}
	assert(saveRoot.isObject());
	Value ents = saveRoot["Entities"];
	assert(ents.isArray());
	for (Value ent : ents) {
		Value assetVal = ent["Asset"];
		Value position = ent["Position"];
		Value rotation = ent["Rotation"];
		assert(assetVal.isString());
		Asset *asset = getAsset(assetVal.asString());
		if (asset == NULL) {
			//ignore invalid asset
			cout << "Invalid asset: " << assetVal.asString() << endl;
			continue;
		}
		btTransform orientation(parseQuaternion(rotation), parseVector3(position));
		entities.push_back(asset->createEntity(orientation, id++));
	}
	return entities;
}

AssetPack::~AssetPack() {

}

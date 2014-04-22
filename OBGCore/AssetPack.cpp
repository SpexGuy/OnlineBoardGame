#include <assert.h>
#include <fstream>
#include <iostream>
#include "Asset.h"
#include "AssetPack.h"
#include "CollisionMeshLoader.h"
#include "CollisionShapeInflater.h"
#include "Entity.h"
#include "json.h"
#include "ShakeStrategy.h"

using namespace std;
using Json::Value;
using Json::ValueType;

bool parseQuaternion(const Value &val, btQuaternion &retQuat) {
	if (!(val.isArray() && val.size() == 4)) return false;
	if (!val[0].isConvertibleTo(ValueType::realValue)) return false;
	if (!val[1].isConvertibleTo(ValueType::realValue)) return false;
	if (!val[2].isConvertibleTo(ValueType::realValue)) return false;
	if (!val[3].isConvertibleTo(ValueType::realValue)) return false;
	retQuat.setValue(val[0].asDouble(), val[1].asDouble(),
					 val[2].asDouble(), val[3].asDouble());
	return true;
}

bool parseVector3(const Value &val, btVector3 &retVec) {
	if (!val.isArray() && val.size() == 3) return false;
	if (!val[0].isConvertibleTo(ValueType::realValue)) return false;
	if (!val[1].isConvertibleTo(ValueType::realValue)) return false;
	if (!val[2].isConvertibleTo(ValueType::realValue)) return false;
	retVec.setValue(val[0].asDouble(), val[1].asDouble(),
					val[2].asDouble());
	return true;
}

AssetPack::AssetPack() {}

bool AssetPack::initialize(const Json::Value &root) {
	if (!root.isObject()) return false;
	Value version = root["Version"];
	Value gameName = root["GameName"];
	Value defaultSave = root["EntityLocationsFile"];
	Value assetRoot = root["Assets"];
	
	if (!version.isConvertibleTo(ValueType::realValue)) return false;
	if (!gameName.isString()) return false;
	if (!defaultSave.isString()) return false;
	if (!assetRoot.isArray()) return false;

	this->version = version.asDouble();
	this->gameName = gameName.asString();
	this->defaultSaveFile = defaultSave.asString();
	this->assetRoot = assetRoot;
	Value assetName;
	for (Value asset : assetRoot) {
		if (!asset.isObject()) continue;
		assetName = asset["Name"];
		if (!assetName.isString()) continue;
		if (assetValues.find(assetName.asString()) != assetValues.end()) {
			cout << "Asset pack contains duplicated asset: " << assetName.asString() <<". Ignoring duplicate." << endl;
			continue;
		}
		assetValues[assetName.asString()] = asset;
	}
	return true;
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
	if (!group.isConvertibleTo(ValueType::stringValue)) return NULL;
	if (!mass.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterX.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterY.isConvertibleTo(ValueType::realValue)) return NULL;
	if (!massCenterZ.isConvertibleTo(ValueType::realValue)) return NULL;

	CollisionShapeInflater *shape;
	btTransform transform;
	if (!parseCollider(collider, &transform, &shape))
		return NULL;

	ShakeStrategy *shake;
	if (!getShakeStrategy(shakeType, &shake))
		return NULL;

	return new Asset(
		name,
		group.asString(),
		mass.asDouble(),
		btVector3(massCenterX.asDouble(),
				  massCenterY.asDouble(),
				  massCenterZ.asDouble()),
		transform, shape, shake);
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
	if (!collider.isObject()) return false;
	Value type = collider["Type"];
	Value position = collider["Position"];
	Value rotation = collider["Rotation"];
	if (!type.isString()) return false;
	btVector3 pos;
	btQuaternion rot;
	if (!parseQuaternion(rotation, rot))
		return false;
	if (!parseVector3(position, pos))
		return false;
	*retTransform = btTransform(rot, pos);


	if (type.asString() == string("Sphere")) {
		Value radius = collider["Radius"];
		if (!radius.isConvertibleTo(ValueType::realValue))
			return false;
		*retShape = new SphereInflater(radius.asDouble());
		return true;
	} else if (type.asString() == string("Box")) {
		Value halfExtents = collider["HalfExtents"];
		btVector3 vec;
		if (!parseVector3(halfExtents, vec)) return false;
		*retShape = new BoxInflater(vec);
		return true;
	} else if (type.asString() == string("Cylinder")) {
		Value halfExtents = collider["HalfExtents"];
		btVector3 vec;
		if (!parseVector3(halfExtents, vec)) return false;
		*retShape = new CylinderInflater(vec);
		return true;
	} else if (type.asString() == string("Capsule")) {
		Value radius = collider["Radius"];
		Value distance = collider["Distance"];
		if (!radius.isConvertibleTo(ValueType::realValue)) return false;
		if (!distance.isConvertibleTo(ValueType::realValue)) return false;
		*retShape = new CapsuleInflater(radius.asDouble(), distance.asDouble());
		return true;
	} else if (type.asString() == string("Cone")) {
		Value radius = collider["Radius"];
		Value height = collider["Height"];
		if (!radius.isConvertibleTo(ValueType::realValue)) return false;
		if (!height.isConvertibleTo(ValueType::realValue)) return false;
		*retShape = new ConeInflater(radius.asDouble(), height.asDouble());
		return true;
	} else if (type.asString() == string("MultiSphere")) {
		Value radiiVal = collider["Radii"];
		Value positionsVal = collider["Positions"];
		if (!radiiVal.isArray()) return false;
		if (!positionsVal.isArray()) return false;
		if (!(radiiVal.size() > 0)) return false;
		if (!(radiiVal.size() == positionsVal.size())) return false;
		int num = radiiVal.size();
		btVector3 *positions = new btVector3[num];
		btScalar *radii = new btScalar[num];
		for (int c = 0; c < num; c++) {
			if (!radiiVal[c].isConvertibleTo(ValueType::realValue))
				return false;
			if (!parseVector3(positionsVal[c], positions[c]))
				return false;
			radii[c] = radiiVal[c].asDouble();
		}
		*retShape = new MultiSphereInflater(positions, radii, num);
		//positions and radii are deleted by the inflater
		return true;
	} else if (type.asString() == string("ConvexHull")) {
		Value file = collider["File"];
		if (!file.isString()) return false;;
		CollisionShapeInflater *shape = getCollider(file.asString());
		*retShape = shape;
		return shape != NULL;
	} else if (type.asString() == string("Empty")) {
		*retShape = new EmptyInflater();
		return true;
	} else if (type.asString() == string("Compound")) {
		Value colliders = collider["Colliders"];
		if (!colliders.isArray()) return false;
		if (!(colliders.size() > 0)) return false;
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

bool AssetPack::getShakeStrategy(const Value &input, ShakeStrategy **retStrat) {
	if (!input.isString())
		return false;
	string type = input.asString();
	if (type == string("None")) {
		*retStrat = ShakeStrategy::defaultShakeStrategy;
		return true;
	} else if (type == string("Spin")) {
		static ShakeStrategy *spinStrategy = new RandomizeRotationShakeStrategy();
		*retStrat = spinStrategy;
		return true;
	}
	return false;
}

bool AssetPack::loadGame(vector<Entity *> &entities) {
	return loadGame(entities, defaultSaveFile);
}

bool AssetPack::loadGame(vector<Entity *> &entities, const string &saveFile) {
	int id = 1;
	Json::Reader reader;
	Value saveRoot;
	ifstream file(saveFile, ios::in);
	if (!file) {
		cout << "Could not open entity file: " << saveFile << endl;
		return false;
	}
	if (!reader.parse(file, saveRoot)) {
		cout << "Could not parse entity file: " << saveFile << endl;
		file.close();
		return false;
	}
	if (!saveRoot.isObject()) return false;
	Value ents = saveRoot["Entities"];
	if (!ents.isArray()) return false;
	for (Value ent : ents) {
		Value assetVal = ent["Asset"];
		Value position = ent["Position"];
		Value rotation = ent["Rotation"];
		if (!assetVal.isString()) return false;
		Asset *asset = getAsset(assetVal.asString());
		if (asset == NULL) {
			//ignore invalid asset
			cout << "Invalid asset: " << assetVal.asString() << endl;
			continue;
		}
		btVector3 pos;
		btQuaternion rot;
		if (!parseQuaternion(rotation, rot)) continue;
		if (!parseVector3(position, pos)) continue;
		entities.push_back(asset->createEntity(btTransform(rot, pos), id++));
	}
	return true;
}

AssetPack::~AssetPack() {

}

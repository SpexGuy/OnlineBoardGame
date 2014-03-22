#include <assert.h>
#include <iostream>
#include <fstream>
#include "Asset.h"
#include "AssetPack.h"
#include "Entity.h"
#include "json.h"

using namespace std;
using Json::Value;
using Json::ValueType;

AssetPack::AssetPack(const Json::Value &root) {
	assert(root.isObject());
	Value version = root["Version"];
	Value gameName = root["GameName"];
	Value defaultSave = root["EntityLocationsFile"];
	Value assetRoot = root["Assets"];
	
	assert(version.isConvertibleTo(ValueType::realValue));
	assert(gameName.isString());
	assert(defaultSave.isString());
	assert(assetRoot.isObject());

	this->version = version.asDouble();
	this->gameName = gameName.asString();
	this->defaultSaveFile = defaultSave.asString();
	this->assetRoot = assetRoot;
}

Asset *AssetPack::makeAsset(const string &name) {
	Value asset = assetRoot[name];
	if (!asset.isObject()) {
		cout << "Asset " << name << " is not an object!" << endl;
		return NULL;
	}

	Value group = asset["Group"];
	Value stackType = asset["StackType"];
	Value shakeType = asset["ShakeType"];
	Value mass = asset["Mass"];
	Value massCenterX = asset["CoMx"];
	Value massCenterY = asset["CoMy"];
	Value massCenterZ = asset["CoMz"];
	Value colliders = asset["Colliders"];
	assert(group.isConvertibleTo(ValueType::stringValue));
	assert(stackType.isNull());
	assert(shakeType.isNull());
	assert(colliders.isArray());
	assert(mass.isConvertibleTo(ValueType::realValue));
	assert(massCenterX.isConvertibleTo(ValueType::realValue));
	assert(massCenterY.isConvertibleTo(ValueType::realValue));
	assert(massCenterZ.isConvertibleTo(ValueType::realValue));

	for (Value collider : colliders) {
		assert(collider.isObject());
		Value colliderFile = collider["Name"];
		assert(colliderFile.isString());
		//TODO: colliders
	}
	//TODO: make Asset
	return NULL;
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

vector<Entity *> AssetPack::loadGame() {
	return loadGame(defaultSaveFile);
}

vector<Entity *> AssetPack::loadGame(const string &saveFile) {
	vector<Entity *> entities;
	Json::Reader reader;
	Value saveRoot;
	ifstream file(saveFile, ios::in);
	if (!file) {
		cout << "Could not open entity file " << saveFile << endl;
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
		Value posX = ent["posX"];
		Value posY = ent["posY"];
		Value posZ = ent["posZ"];
		Value rotX = ent["rotX"];
		Value rotY = ent["rotY"];
		Value rotZ = ent["rotZ"];
		assert(assetVal.isString());
		assert(posX.isConvertibleTo(ValueType::realValue));
		assert(posY.isConvertibleTo(ValueType::realValue));
		assert(posZ.isConvertibleTo(ValueType::realValue));
		assert(rotX.isConvertibleTo(ValueType::realValue));
		assert(rotY.isConvertibleTo(ValueType::realValue));
		assert(rotZ.isConvertibleTo(ValueType::realValue));
		Asset *asset = getAsset(assetVal.asString());
		if (asset == NULL) {
			continue;
		}
		entities.push_back(asset->createEntity(btVector3(posX.asDouble(), posY.asDouble(), posZ.asDouble())));
		//TODO: add rotation
	}
	return entities;
}

AssetPack::~AssetPack() {

}

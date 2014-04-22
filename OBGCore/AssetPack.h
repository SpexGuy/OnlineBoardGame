#pragma once
#include <map>
#include <string>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "json.h"

class Asset;
class CollisionShapeInflater;
class Entity;
class ShakeStrategy;

class AssetPack {
protected:
	double version;
	std::string gameName;
	std::string defaultSaveFile;
	std::map<std::string, Json::Value> assetValues;
	std::map<std::string, Asset *> assets;
	std::map<std::string, std::vector<btVector3> *> colliders;
	Json::Value assetRoot;

	virtual Asset *makeAsset(const std::string &name);
	virtual Asset *getAsset(const std::string &name);

	virtual CollisionShapeInflater *getCollider(const std::string &name);
	virtual bool parseCollider(const Json::Value &collider, btTransform *retTransform, CollisionShapeInflater **retShape);
	virtual bool getShakeStrategy(const Json::Value &inputType, ShakeStrategy **retStrategy);

public:
	AssetPack();

	virtual bool initialize(const Json::Value &root);

	virtual bool loadGame(std::vector<Entity *> &entities);
	virtual bool loadGame(std::vector<Entity *> &entities, const std::string &saveFile);

	virtual ~AssetPack();
};

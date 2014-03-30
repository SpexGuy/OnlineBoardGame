#pragma once
#include <map>
#include <string>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "json.h"

class Asset;
class CollisionShapeInflater;
class Entity;

class AssetPack {
protected:
	double version;
	std::string gameName;
	std::string defaultSaveFile;
	std::map<std::string, Asset *> assets;
	std::map<std::string, std::vector<btVector3> *> colliders;
	Json::Value assetRoot;

	virtual Asset *makeAsset(const std::string &name);
	virtual Asset *getAsset(const std::string &name);

	virtual CollisionShapeInflater *getCollider(const std::string &name);
	virtual bool parseCollider(const Json::Value &collider, btTransform *retTransform, CollisionShapeInflater **retShape);

public:
	AssetPack(const Json::Value &root);

	virtual std::vector<Entity *> loadGame();
	virtual std::vector<Entity *> loadGame(const std::string &saveFile);

	virtual ~AssetPack();
};

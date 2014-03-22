#pragma once
#include <map>
#include <string>
#include <vector>
#include "json.h"

class Asset;
class Entity;

class AssetPack {
protected:
	double version;
	std::string gameName;
	std::string defaultSaveFile;
	std::map<std::string, Asset *> assets;
	Json::Value assetRoot;

	virtual Asset *makeAsset(const std::string &name);
	virtual Asset *getAsset(const std::string &name);

public:
	AssetPack(const Json::Value &root);

	virtual std::vector<Entity *> loadGame();
	virtual std::vector<Entity *> loadGame(const std::string &saveFile);

	virtual ~AssetPack();
};

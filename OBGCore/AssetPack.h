#pragma once
#include <vector>
#include <string>

class Asset;
class Entity;

class AssetPack {
protected:
	std::string gameName;
	long int versionHash;
	std::vector<Asset *> assets;

public:
	AssetPack(std::string gameName);

	virtual std::vector<Entity *> loadGame();

	virtual ~AssetPack();
};

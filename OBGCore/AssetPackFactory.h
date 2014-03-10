#pragma once
#include <string>

class AssetPack;

class AssetPackFactory {
public:
	virtual AssetPack *load(std::string name);
	virtual ~AssetPackFactory();
};

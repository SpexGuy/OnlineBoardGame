#pragma once
#include <AssetPack.h>
#include "GraphicsEntity.h"

class GraphicsAssetPack :
	public AssetPack
{
public:
	virtual std::vector<Entity *> loadGame();
};

#pragma once
#include <AssetPackFactory.h>
#include "GraphicsAssetPack.h"

class GraphicsAssetPackFactory :
	public AssetPackFactory
{
	virtual GraphicsAssetPack *load(std::string name);
};

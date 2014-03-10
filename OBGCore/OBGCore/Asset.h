#pragma once

class btVector3;
class Entity;

class Asset {
protected:
	int assetPackIndex;
	int group;
public:

	Asset(int assetPackIndex, int group) :
		assetPackIndex(assetPackIndex),
		group(group)
	{}

	virtual Entity *createEntity(btVector3 &position);
	
	virtual ~Asset();
};

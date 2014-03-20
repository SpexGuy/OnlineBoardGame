#include "AssetPack.h"

using namespace std;

AssetPack::AssetPack(const string &gameName) {
	this->gameName = gameName;
	this->versionHash = 0; //TODO
}

vector<Entity *> AssetPack::loadGame() {
	vector<Entity *> entities;
	//TODO: load them
	return entities;
}

AssetPack::~AssetPack() {

}

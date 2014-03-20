#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <AssetPack.h>

class GraphicsMesh;

class GraphicsAssetPack :
	public AssetPack
{
private:
	
protected:
	std::map<std::string, GraphicsMesh *> meshes;

	virtual void downloadFile(const std::string &filename);
public:
	GraphicsAssetPack(const std::string &gameName) :
		AssetPack(gameName) {}

	virtual std::ifstream *getFile(const std::string &filename);
	virtual GraphicsMesh *getMesh(const std::string &filename);

	//virtual std::vector<Entity *> loadGame();

	virtual ~GraphicsAssetPack();
};

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <AssetPack.h>
#include <GLSLProgram.h>
#include <GraphicsMesh.h>
#include <ILContainer.h>

class GraphicsAsset;

class GraphicsAssetPack :
	public AssetPack
{
private:
	
protected:
	std::map<std::string, GraphicsMesh *> meshes;
	std::map<std::string, ILContainer *> images;

	GLSLProgram *texShader;

	virtual void downloadFile(const std::string &filename);
	virtual GraphicsAsset *makeAsset(const std::string &name);

public:
	GraphicsAssetPack();

	virtual bool initialize(const Json::Value &root);

	virtual std::ifstream *getFile(const std::string &filename);
	virtual GraphicsMesh *getMesh(const std::string &filename);
	virtual ILContainer *getImage(const std::string &filename);

	virtual ~GraphicsAssetPack();
};

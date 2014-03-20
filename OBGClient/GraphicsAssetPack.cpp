#include <GraphicsMesh.h>
#include "ClientConnection.h"
#include "GameManager.h"
#include "GraphicsAssetPack.h"

using namespace std;

void GraphicsAssetPack::downloadFile(const string &file) {
	GameManager::inst()->getConnection()->downloadFile(file);
}

ifstream *GraphicsAssetPack::getFile(const string &filename) {
	ifstream *stream = new ifstream(filename, ios::in);
	if (!*stream) {
		delete stream;
		downloadFile(filename);
		stream = new ifstream(filename, ios::in);
		if (!*stream) {
			delete stream;
			return NULL;
		}
	}
	return stream;
}

GraphicsMesh *GraphicsAssetPack::getMesh(const string &filename) {
	map<string, GraphicsMesh *>::const_iterator pos = meshes.find(filename);
	GraphicsMesh *mesh;
	if (pos == meshes.end()) {
		//load the mesh
		ifstream *file = getFile(filename);
		if (file == NULL) {
			cout << "Could not load file " << filename << endl;
			return NULL;
		}
		mesh = GraphicsMesh::loadMesh(file);
		file->close();
		delete file;
		if (mesh == NULL) {
			cout << "Could not load mesh from " << filename << endl;
			return NULL;
		}
		meshes[filename] = mesh;
	} else {
		mesh = pos->second;
	}
	return mesh;
}

GraphicsAssetPack::~GraphicsAssetPack() {
	//free all allocated resources
	for (auto iter : meshes) {
		delete iter.second;
	}
	meshes.clear();
}

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <GLSLProgram.h>
#include <GraphicsMesh.h>
#include <MatrixShader.h>
#include <ShaderMaterial.h>
#include <Vertex.h>
#include "GameManager.h"
#include "GraphicsAsset.h"
#include "GraphicsManager.h"

using namespace std;
using namespace glm;


GameManager::GameManager(int argc, char *argv[]) {
	graphicsManager = new GraphicsManager(argc, argv);

	vector<VertexPNT> verts;
	verts.push_back(VertexPNT(vec3(-1,-1,0),vec3(0,0,1),vec2(0,0)));
	verts.push_back(VertexPNT(vec3(0,1,0),vec3(0,1,0),vec2(0,0)));
	verts.push_back(VertexPNT(vec3(1,-1,0),vec3(1,0,0),vec2(0,0)));

	vector<GLuint> trigs;
	trigs.push_back(0);
	trigs.push_back(1);
	trigs.push_back(2);

	GraphicsMesh *mesh = new GraphicsMesh(verts, trigs);

	GLSLProgram *shader = new MatrixShader();
	shader->compileShader("NormalShader.vert");
	shader->compileShader("NormalShader.frag");
	shader->link();

	Material *material = new ShaderMaterial(shader);

	GraphicsAsset *asset = new GraphicsAsset(mesh, material);
	GraphicsEntity *entity = asset->createEntity(btVector3(0,0,0));
	graphicsManager->addRenderable(entity);

	glDisable(GL_CULL_FACE);
	glClearColor(0.2f,0.2f,0.2f,1.0f);
}

void GameManager::run() {
	graphicsManager->update();
}

GameManager::~GameManager() {

}
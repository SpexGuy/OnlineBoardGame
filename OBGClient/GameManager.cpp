#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <GLSLProgram.h>
#include <GraphicsMesh.h>
#include <ILTexture.h>
#include <Texture.h>
#include <TextureGroup.h>
#include <TextureMaterial.h>
#include <TextureShader.h>
#include <Vertex.h>
#include "GameManager.h"
#include "GraphicsAsset.h"
#include "GraphicsManager.h"

#define PERIOD (int)(1000.0/60.0)

using namespace std;
using namespace glm;


GameManager *instance;

void displayFunc() {
	//cout << "display" << endl;
	instance->display();
}

void updateFunc(int value) {
	instance->update();
}

void closeFunc() {
	instance->close();
}

GameManager::GameManager(int argc, char *argv[]) {
	running = false;
	graphicsManager = new GraphicsManager(argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutCloseFunc(closeFunc);
	glutDisplayFunc(displayFunc);

	GraphicsMesh *mesh = GraphicsMesh::loadMesh("badEarth.obj");

	GLSLProgram *shader = new TextureShader();
	shader->compileShader("NormalShader.vert");
	shader->compileShader("NormalShader.frag");
	shader->link();

	Texture *tex = new ILTexture("super_earth.jpg", 0);

	Material *material = new TextureMaterial(tex, shader);

	GraphicsAsset *asset = new GraphicsAsset(mesh, material);
	GraphicsEntity *entity = asset->createEntity(btVector3(0,0,0));
	graphicsManager->addRenderable(entity);

	instance = this;
}

void GameManager::run() {
	running = true;
	glutTimerFunc(PERIOD, updateFunc, 0);
	glutMainLoop();
}

void GameManager::update() {
	if (running) {
		glutTimerFunc(PERIOD, updateFunc, 0);
		glutPostRedisplay();
	}
}

void GameManager::display() {
	if (running) {
		graphicsManager->display();
	}
}

void GameManager::close() {
	running = false;
	glutLeaveMainLoop();
}

GameManager::~GameManager() {

}
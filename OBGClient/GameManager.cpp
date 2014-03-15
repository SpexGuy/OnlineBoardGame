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
#define ESCAPE_CHAR 0x1B

using namespace std;
using namespace glm;


GameManager *instance;

void updateFunc(int value) {
	instance->update();
}

void displayFunc() {
	instance->display();
}

void keyboardFunc(unsigned char c, int x, int y) {
	instance->keyPressed(c, x, y);
}

void specialFunc(int k, int x, int y) {
	instance->specialKeyPressed(k, x, y);
}

void mouseFunc(int button, int state, int x, int y) {
	instance->mousePressed(button, state, x, y);
}

void motionFunc(int x, int y) {
	instance->mouseDragged(x, y);
}

void passiveMotionFunc(int x, int y) {
	instance->mouseMoved(x, y);
}

void visibilityFunc(int state) {
	instance->visibilityChanged(state);
}

void reshapeFunc(int x, int y) {
	instance->reshape(x, y);
}

void closeFunc() {
	instance->close();
}

GameManager::GameManager(int argc, char *argv[]) {
	running = false;
	graphicsManager = new GraphicsManager(argc, argv);
}

void GameManager::run() {
	running = true;
	visible = true;

	graphicsManager->start();

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

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(passiveMotionFunc);
	glutVisibilityFunc(visibilityFunc);
	glutReshapeFunc(reshapeFunc);
	glutCloseFunc(closeFunc);
	glutTimerFunc(PERIOD, updateFunc, 0);

	instance = this;

	glutMainLoop();
}

void GameManager::update() {
	if (running) {
		glutTimerFunc(PERIOD, updateFunc, 0);
		glutPostRedisplay();
	}
}

void GameManager::display() {
	if (running && visible) {
		graphicsManager->display();
	}
}

void GameManager::keyPressed(unsigned char c, int x, int y) {
	switch(c) {
	case ESCAPE_CHAR:
		if (glutGet(GLUT_FULL_SCREEN)) {
			glutLeaveFullScreen();
			break;
		} //else fall through
	case 'X':
	case 'x':
		close();
		break;
	default:
		cout << "Unknown character pressed: " << c << " (" << int(c) << ")" << endl;
	}
}

void GameManager::specialKeyPressed(int k, int x, int y) {
	switch(k) {
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	default:
		cout << "Unknown special key: " << k << endl;
	}
}

void GameManager::mousePressed(int button, int state, int x, int y) {
	switch(button) {
	case 0: //left click
		break;
	case 1: //right click
		break;
	case 2: //middle click
		break;
	case 3: //back button
		break;
	case 4: //forward button
		break;
	default:
		cout << "TOO MANY BUTTONS ON THAT MOUSE!" << endl;
	}
}

void GameManager::mouseDragged(int x, int y) {

}

void GameManager::mouseMoved(int x, int y) {

}

void GameManager::visibilityChanged(int state) {
	visible = (state != GLUT_NOT_VISIBLE);
}

void GameManager::reshape(int x, int y) {
	graphicsManager->reshape(x, y);
}

void GameManager::close() {
	running = false;
	glutLeaveMainLoop();
}

GameManager::~GameManager() {

}
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <json.h>
#include <Socket.h>

#include <EntityManager.h>
#include <GLSLProgram.h>
#include <GraphicsMesh.h>
#include <ILTexture.h>
#include <Texture.h>
#include <TextureGroup.h>
#include <TextureMaterial.h>
#include <TextureShader.h>
#include <Vertex.h>

#include "ChatBox.h"
#include "ClientConnection.h"
#include "GameManager.h"
#include "GraphicsAsset.h"
#include "GraphicsAssetPack.h"
#include "GraphicsManager.h"
#include "MousePointer.h"
#include "UserInputHandler.h"

#define PERIOD (int)(1000.0/60.0)
#define ESCAPE_CHAR 0x1B

using namespace std;
using namespace glm;


GameManager *instance = NULL;

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


GameManager *GameManager::inst() {
	return instance;
}


GameManager::GameManager(int argc, char *argv[]) {
	running = false;
	//init everything
	SocketInit();
	graphicsManager = new GraphicsManager(argc, argv);
	connection = new ClientConnection("127.0.0.1", 0xABC0);
	inputHandler = new UserInputHandler();
	entityManager = new EntityManager();
	//set up listeners
	connection->registerMessageListener(inputHandler->getChatBox());
	inputHandler->getChatBox()->registerMessageListener(connection);
	connection->registerPhysicsUpdateListener(entityManager);
	inputHandler->registerInteractionListener(connection);
	inputHandler->registerInteractionListener(entityManager);
	//entityManager->registerPhysicsUpdateListener(entityManager);
	//provide global access point
	assert(instance == NULL);
	instance = this;
}

void GameManager::run() {
	running = true;
	visible = true;

	graphicsManager->start();
	connection->start();
	inputHandler->start();

	graphicsManager->addRenderable(inputHandler->getChatBox());
	graphicsManager->addRenderable(inputHandler->getMousePointer());

	ifstream file("assets.json");
	assert(file);
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(file, root)) {
		cout << "Could not parse JSON file" << endl;
		file.close();
		assert(false);
	}
	file.close();
	GraphicsAssetPack *pack = new GraphicsAssetPack(root);
	vector<Entity *> entities = pack->loadGame();
	assert(entities.size() > 0);
	//((GraphicsEntity *) entities[0])->render();
	for (Entity *entity : entities) {
		graphicsManager->addRenderable((GraphicsEntity *)entity);
		entityManager->addEntity(entity);
		//entity->hide();
	}

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

	entityManager->start();

	glutMainLoop();
}

void GameManager::update() {
	if (running) {
		glutTimerFunc(PERIOD, updateFunc, 0);
		inputHandler->update();
		entityManager->update();
		entityManager->createPhysicsUpdates();
		glutPostRedisplay();
	}
}

void GameManager::display() {
	if (running && visible) {
		graphicsManager->display();
	}
}

void GameManager::keyPressed(unsigned char c, int x, int y) {
	inputHandler->keyPressed(c, x, y);
}

void GameManager::specialKeyPressed(int k, int x, int y) {
	inputHandler->specialKeyPressed(k,x,y);
}

void GameManager::mousePressed(int button, int state, int x, int y) {
	inputHandler->mousePressed(button, state, x, y);
}

void GameManager::mouseDragged(int x, int y) {
	inputHandler->mouseDragged(x, y);
}

void GameManager::mouseMoved(int x, int y) {
	inputHandler->mouseDragged(x, y);
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
	delete graphicsManager;
	delete connection;
	SocketClose();
}
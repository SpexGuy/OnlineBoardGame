#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <json.h>
#include <Socket.h>

#include <GLErrorCheck.h>
#include <GLSLProgram.h>
#include <GraphicsContext.h>
#include <GraphicsMesh.h>
#include <ILTexture.h>
#include <Texture.h>
#include <TextureGroup.h>
#include <TextureMaterial.h>
#include <TextureShader.h>
#include <Vertex.h>

#include "ChatBox.h"
#include "ClientConnection.h"
#include "ClientEntityManager.h"
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

void keyboardFunc(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		instance->keyPressed(key, scancode, mods);
	} else if (action == GLFW_RELEASE) {
		instance->keyReleased(key, scancode, mods);
	}
}

void mouseFunc(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		instance->mousePressed(button, mods);
	} else if (action == GLFW_RELEASE) {
		instance->mouseReleased(button, mods);
	}
}

void motionFunc(GLFWwindow *window, double x, double y) {
	instance->mouseMoved(int(x), int(y));
}

GameManager *GameManager::inst() {
	return instance;
}


GameManager::GameManager(int argc, char *argv[]) {
	//parse arguments
	if (argc > 1) {
		serverIp = Address::TCPAddress(string(argv[1]), 0xABC0);
	}
	if (!serverIp) {
		cout << "Connecting to localhost" << endl;
		serverIp = Address::TCPAddress(0x7F000001, 0xABC0);
	}
	//init everything
	SocketInit();
	graphicsManager = new GraphicsManager(argc, argv);
	connection = new ClientConnection();
	inputHandler = new UserInputHandler();
	entityManager = new ClientEntityManager();
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
	graphicsManager->start();

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
	for (Entity *entity : entities) {
		graphicsManager->addRenderable((GraphicsEntity *)entity);
		entityManager->addEntity(entity);
	}

	GraphicsContext::inst()->setKeyCallback(keyboardFunc);
	GraphicsContext::inst()->setMouseCallback(mouseFunc);
	GraphicsContext::inst()->setMouseMotionCallback(motionFunc);

	entityManager->start();
	connected = connection->connect(clock(), serverIp, 0xABC8);
	if (!connected) {
		cout << "Could not connect to server. Continuing..." << endl;
	} else {
		connection->start();
	}
	inputHandler->start();

	graphicsManager->addRenderable(inputHandler->getChatBox());
	graphicsManager->addRenderable(inputHandler->getMousePointer());

	//TODO: do this in a different thread so that
	//windows freezing the main thread on ui changes
	//does not cause a connection timeout
	while (!GraphicsContext::inst()->shouldCloseWindow()) {
		clock_t time(clock());
		update(time);
		//poll events at least once until time for next frame 
		do { GraphicsContext::inst()->pollEvents(); }
			while(clock() - time < PERIOD);
	}
	GraphicsContext::inst()->closeWindow();
}

void GameManager::update(int time) {
	inputHandler->update(time);
	entityManager->update(time);
	graphicsManager->update(time);
	if (connected)
		connected = connection->update(time);
	display();
}

void GameManager::display() {
	if (GraphicsContext::inst()->isVisible()) {
		checkError("Before display");
		graphicsManager->display();
	}
}

void GameManager::keyPressed(int k, int s, int m) {
	inputHandler->keyPressed(k,s,m);
}

void GameManager::keyReleased(int k, int s, int m) {
	inputHandler->keyReleased(k,s,m);
}

void GameManager::mousePressed(int button, int mods) {
	inputHandler->mousePressed(button, mods);
}

void GameManager::mouseReleased(int button, int mods) {
	inputHandler->mouseReleased(button, mods);
}

void GameManager::mouseMoved(int x, int y) {
	inputHandler->mouseMoved(x, y);
}

GameManager::~GameManager() {
	delete entityManager;
	delete inputHandler;
	delete graphicsManager;
	delete connection;
	SocketClose();
}
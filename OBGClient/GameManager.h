#pragma once

class GraphicsManager;
class EntityManager;
class Connection;
class UserInputHandler;

class GameManager {
protected:
	GraphicsManager *graphicsManager;
	EntityManager *entityManager;
	Connection *connection;
	UserInputHandler *inputHandler;

public:
	GameManager();
	virtual void run();
	virtual ~GameManager();
};

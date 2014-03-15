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

	bool running;
	bool visible;

public:
	GameManager(int argc, char *argv[]);
	virtual void run();
	virtual void update();
	virtual void display();
	virtual void keyPressed(unsigned char c, int x, int y);
	virtual void specialKeyPressed(int k, int x, int y);
	virtual void mousePressed(int button, int state, int x, int y);
	virtual void mouseDragged(int x, int y);
	virtual void mouseMoved(int x, int y);
	virtual void visibilityChanged(int state);
	virtual void reshape(int x, int y);
	virtual void close();
	virtual ~GameManager();
};

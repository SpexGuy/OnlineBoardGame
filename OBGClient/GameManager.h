#pragma once

class GraphicsManager;
class EntityManager;
class ClientConnection;
class UserInputHandler;

class GameManager {
protected:
	GraphicsManager	 *graphicsManager;
	EntityManager	 *entityManager;
	ClientConnection *connection;
	UserInputHandler *inputHandler;

	bool running;
	bool visible;

public:
	static GameManager *inst();

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

	//accessors
	inline GraphicsManager	*getGraphicsManager()	{ return graphicsManager;	}
	inline EntityManager	*getEntityManager()		{ return entityManager;		}
	inline ClientConnection	*getConnection()		{ return connection;		}
	inline UserInputHandler	*getInputHandler()		{ return inputHandler;		}
};

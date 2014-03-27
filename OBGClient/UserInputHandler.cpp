#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ChatBox.h"
#include "GameManager.h"
#include "UserInputHandler.h"
#include "EntityManager.h"
#include "Interaction.h"
#include "Entity.h"

#define CHAR_ESCAPE 0x1B
#define CHAR_DELETE 0x7F
#define CHAR_BACKSPACE 0x08
#define CHAR_ENTER '\r'
#define MOUSE_HEIGHT 10


using namespace std;

UserInputHandler::UserInputHandler() {
	chat = new ChatBox();
	mouseX = 0.0;
	mouseZ = 0.0;
}

void UserInputHandler::update() {
	vector<int> idList = vector<int>();
	for(int i : lastHeldList) {
		if(find(begin(heldList), end(heldList), i) == end(heldList)) {
			idList.push_back(-i);
		}
	}

	for(int i : heldList) {
		idList.push_back(i);
	}

	lastHeldList = heldList;

	Interaction *interaction = new Interaction(btVector3(mouseX, 0, mouseZ), idList);
	fireInteraction(interaction);
}

void UserInputHandler::keyPressed(unsigned char c, int x, int y) {

	if (chat->isEditMode()) {
		switch(c) {
		case CHAR_ESCAPE:
			chat->cancel();
			break;
		case CHAR_BACKSPACE:
		case CHAR_DELETE:
			chat->popCharacter();
			break;
		case CHAR_ENTER:
			chat->submit();
			break;
		default:
			if (c >= 0x20 && c <= 0x7E)
				chat->pushCharacter(c);
			else
				cout << "Could not type character " << int(c) << endl;
		}
	} else {
		switch(c) {
		case 't':
		case 'T':
		case CHAR_ENTER:
			chat->enterEditMode();
			break;
		case CHAR_ESCAPE:
			if (glutGet(GLUT_FULL_SCREEN)) {
				glutLeaveFullScreen();
				break;
			} //else fall through
		case 'X':
		case 'x':
			GameManager::inst()->close();
			break;
		case 'w':
		case 'W':
			//Rotate -x
			break;
		case 's':
		case 'S':
			//Rotate x
			break;
		case 'a':
		case 'A':
			//Rotate z
			break;
		case 'd':
		case 'D':
			//rotate -z
			break;
		case 'q':
		case 'Q':
			//rotate y
			break;
		case 'e':
		case 'E':
			//rotate -y
			break;
		default:
			cout << "Unknown character pressed: " << c << " (" << int(c) << ")" << endl;
		}
	}
}

void UserInputHandler::specialKeyPressed(int k, int x, int y) {
	switch(k) {
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	default:
		cout << "Unknown special key: " << k << endl;
	}
}

void UserInputHandler::mousePressed(int button, int state, int x, int y) {
	EntityManager* entityManager = GameManager::inst()->getEntityManager();

	switch(button) {
	case 0:{ //left click
		Entity *clickedEntity = entityManager->getIntersectingEntity(btVector3(mouseX, MOUSE_HEIGHT, mouseZ), btVector3(mouseX, 0, mouseZ));
		if(clickedEntity != NULL) {
			heldList.push_back(clickedEntity->getId());
		}
		break;}
	case 1: //right click
		break;
	case 2: //middle click
		break;
	case 3: //back button
		break;
	case 4: //forward button
		break;
	default:
		cout << "TOO MANY BUTTONS ON THAT MOUSE! (button " << button << " was pressed)" << endl;
	}
}

void UserInputHandler::mouseDragged(int x, int y) {
	
}

void UserInputHandler::mouseMoved(int x, int y) {
	double mouseDiffX = x - screenX;
	double mouseDiffY = y - screenY;

	mouseDiffX /= 500;
	mouseDiffY /= 500;

	mouseX += mouseDiffX;
	mouseZ -= mouseDiffY;
}

UserInputHandler::~UserInputHandler() {
	delete chat;
}

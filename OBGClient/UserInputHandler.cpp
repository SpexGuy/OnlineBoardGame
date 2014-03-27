#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ChatBox.h"
#include "GameManager.h"
#include "UserInputHandler.h"
#include "EntityManager.h"
#include "Interaction.h"
#include "Entity.h"
#include "MousePointer.h"

#define CHAR_ESCAPE 0x1B
#define CHAR_DELETE 0x7F
#define CHAR_BACKSPACE 0x08
#define CHAR_ENTER '\r'


using namespace std;
using namespace glm;

UserInputHandler::UserInputHandler() {
	chat = new ChatBox();
}

void UserInputHandler::start() {
	pointer = new MousePointer();
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
	
	vec3 worldPos = pointer->getWorldPos();
	btVector3 btPos(worldPos.x, 1, worldPos.z);
	Interaction *interaction = new Interaction(btPos, idList);
	fireInteraction(interaction);
	delete interaction;
}

void UserInputHandler::keyPressed(unsigned char c, int x, int y) {
	pointer->setScreenPos(ivec2(x, y));

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
	pointer->setScreenPos(ivec2(x, y));
	switch(k) {
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	default:
		cout << "Unknown special key: " << k << endl;
	}
}

void UserInputHandler::mousePressed(int button, int state, int x, int y) {
	pointer->setScreenPos(ivec2(x, y));
	EntityManager* entityManager = GameManager::inst()->getEntityManager();

	switch(button) {
	case 0:{ //left click
		vec3 pos = pointer->getWorldPos();
		Entity *clickedEntity = entityManager->getIntersectingEntity(btVector3(pos.x, pos.y, pos.z), btVector3(pos.x, 0, pos.z));
		if(clickedEntity != NULL) {
			cout << "Clicked on entity " << clickedEntity->getId() << endl;
			heldList.push_back(clickedEntity->getId());
		} else {
			cout << "Clicked on nothing" << endl;
		}
		break;
	}
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
	pointer->setScreenPos(ivec2(x, y));
}

void UserInputHandler::mouseMoved(int x, int y) {
	pointer->setScreenPos(ivec2(x, y));
}

UserInputHandler::~UserInputHandler() {
	delete chat;
}

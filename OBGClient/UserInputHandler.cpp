#include <iostream>
#include <GraphicsContext.h>
#include "ChatBox.h"
#include "ClientEntityManager.h"
#include "Entity.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "Interaction.h"
#include "MousePointer.h"
#include "UserInputHandler.h"

#define CHAR_ESCAPE 0x1B
#define CHAR_DELETE 0x7F
#define CHAR_BACKSPACE 0x08
#define CHAR_ENTER '\r'
#define SHAKE_SIZE 40
#define SHAKE_THRESHOLD 2

#define ROTATIONAL_VELOCITY 100

using namespace std;
using namespace glm;

UserInputHandler::UserInputHandler() {
	chat = new ChatBox();
	wireframe = false;
	flags = 0;
	shakes = list<bool>();
	shakeCount = 0;
}

void UserInputHandler::start() {
	pointer = new MousePointer();
	lastMouse = glm::vec2(pointer->getScreenPos().x, pointer->getScreenPos().y);
}

void UserInputHandler::update(int time) {
	pointer->update(time);
	vector<int> idList = vector<int>();
	for(int i : lastHeldList) {
		if(find(begin(heldList), end(heldList), i) == end(heldList)) {
			idList.push_back(-i);
		}
	}

	for(int i : heldList) {
		idList.push_back(i);
	}

	glm::vec2 mouse = glm::vec2(pointer->getScreenPos().x, pointer->getScreenPos().y);
	glm::vec2 mouseDiff = mouse - lastMouse;

	if(glm::dot(mouseDiff, lastMouseDiff) < 0.0) {
		shakeCount ++;
		shakes.push_back(true);
	} else {
		shakes.push_back(false);
	}

	while(shakes.size() > SHAKE_SIZE) {
		bool pop = *shakes.begin();
		if(pop) shakeCount --;
		shakes.pop_front();
	}

	lastMouse = mouse;
	lastMouseDiff = mouseDiff;
	if(shakeCount >= SHAKE_THRESHOLD) {
		flags |= SHAKING;
	} else {
		flags &= ~SHAKING;
	}

	lastHeldList = heldList;
	
	vec3 worldPos = pointer->getWorldPos();
	btVector3 btPos(worldPos.x, worldPos.y, worldPos.z);
	Interaction *interaction = new Interaction(btPos, idList, flags);
	fireInteraction(interaction);
	delete interaction;
}

void UserInputHandler::keyPressed(int key, int scancode, int mods) {
	if (chat->isEditMode()) {
		switch(key) {
		case GLFW_KEY_ESCAPE:
			chat->cancel();
			break;
		case GLFW_KEY_DELETE:
		case GLFW_KEY_BACKSPACE:
			chat->popCharacter();
			break;
		case GLFW_KEY_ENTER:
			chat->submit();
			break;
		default:
			if (key >= 0x20 && key <= 0x7E)
				chat->pushCharacter(key);
			else
				cout << "Could not type character " << int(key) << endl;
		}
	} else {
		switch(key) {
		case GLFW_KEY_T:
		case GLFW_KEY_ENTER:
			chat->enterEditMode();
			break;
		case GLFW_KEY_ESCAPE:
			//if (GraphicsContext::inst()->isFullscreen()) {
			//	GraphicsContext::inst()->setFullscreen(false);
			//	break;
			//} //else fall through
		case GLFW_KEY_X:
			GraphicsContext::inst()->setShouldCloseWindow(true);
			break;
		case GLFW_KEY_W:
			flags |= ROT_NEG_X;
			break;
		case GLFW_KEY_S:
			flags |= ROT_POS_X;
			break;
		case GLFW_KEY_A:
			flags |= ROT_POS_Z;
			break;
		case GLFW_KEY_D:
			flags |= ROT_NEG_Z;
			break;
		case GLFW_KEY_Q:
			flags |= ROT_POS_Y;
			break;
		case GLFW_KEY_E:
			flags |= ROT_NEG_Y;
			break;
		case GLFW_KEY_R:
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
			break;
		case GLFW_KEY_Z:
			GameManager::inst()->getGraphicsManager()->toggleZoom();
			break;
		case GLFW_KEY_F:
			pointer->goLow();
			break;
		//case GLFW_KEY_F11:
		//	GraphicsContext::inst()->toggleFullscreen();
		//	break;
		default:
			cout << "Unknown key pressed: " << key << endl;
		}
	}
}

void UserInputHandler::keyReleased(int key, int scancode, int mods) {
	switch(key) {
		if(!chat->isEditMode()) {
			case GLFW_KEY_W:
				flags &= ~ROT_NEG_X;
				break;
			case GLFW_KEY_S:
				flags &= ~ROT_POS_X;
				break;
			case GLFW_KEY_A:
				flags &= ~ROT_POS_Z;
				break;
			case GLFW_KEY_D:
				flags &= ~ROT_NEG_Z;
				break;
			case GLFW_KEY_Q:
				flags &= ~ROT_POS_Y;
				break;
			case GLFW_KEY_E:
				flags &= ~ROT_NEG_Y;
				break;
			case GLFW_KEY_F:
				pointer->goHigh();
				break;
		}
	}
}

void UserInputHandler::mousePressed(int button, int mods) {
	ClientEntityManager* entityManager = GameManager::inst()->getEntityManager();

	switch(button) {
	case GLFW_MOUSE_BUTTON_LEFT: {
		vec3 pos = pointer->getWorldPos();
		Entity *clickedEntity = entityManager->getIntersectingEntity(btVector3(pos.x, pos.y, pos.z), btVector3(pos.x, 0, pos.z), vector<int>());
		if(clickedEntity != NULL) {
			int id = clickedEntity->getId();
			heldList.push_back(id);
			cout << "Clicked on entity " << id << endl;
		} else {
			cout << "Clicked on nothing" << endl;
		}
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT: //right click
		if(heldList.size() != 0) {
			vec3 pos = pointer->getWorldPos();
			Entity *clickedEntity = entityManager->getIntersectingEntity(btVector3(pos.x, pos.y, pos.z), btVector3(pos.x, 0, pos.z), lastHeldList);
			if(clickedEntity != NULL) {
				int id = clickedEntity->getId();
				heldList.push_back(id);
			}
		}
		break;
	case GLFW_MOUSE_BUTTON_4:
		if (heldList.size() > 0) {
			heldList.pop_back();
		}
		break;
	default:
		cout << "unsupported mouse button: " << button << endl;
	}
}

void UserInputHandler::mouseReleased(int button, int mods) {
	switch(button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		heldList.clear();
		break;
	}
}

void UserInputHandler::mouseMoved(int x, int y) {
	pointer->setScreenPos(ivec2(x, y));
}

UserInputHandler::~UserInputHandler() {
	delete chat;
	delete pointer;
}

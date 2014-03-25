#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ChatBox.h"
#include "GameManager.h"
#include "UserInputHandler.h"

#define CHAR_ESCAPE 0x1B
#define CHAR_DELETE 0x7F
#define CHAR_BACKSPACE 0x08
#define CHAR_ENTER '\r'



using namespace std;

UserInputHandler::UserInputHandler() {
	chat = new ChatBox();
}

void UserInputHandler::update() {

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
		cout << "TOO MANY BUTTONS ON THAT MOUSE! (button " << button << " was pressed)" << endl;
	}
}

void UserInputHandler::mouseDragged(int x, int y) {
	
}

void UserInputHandler::mouseMoved(int x, int y) {

}

UserInputHandler::~UserInputHandler() {
	delete chat;
}

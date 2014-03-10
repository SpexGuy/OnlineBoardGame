#include "GameManager.h"

int main(int argc, char *argv[]) {
	GameManager *gm = new GameManager();
	gm->run();
	delete gm;
	return 0;
}

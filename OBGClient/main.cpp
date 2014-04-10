#include <iostream>
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[]) {
	GameManager gm(argc, argv);
	gm.run();
	system("pause");
	return 0;
}

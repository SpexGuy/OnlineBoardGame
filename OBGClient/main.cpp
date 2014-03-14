#include <iostream>
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[]) {
	GameManager *gm = new GameManager(argc, argv);
	gm->run();
	cout << "Enter any button to close" << endl;
	char i;
	cin >> i;
	delete gm;
	return 0;
}

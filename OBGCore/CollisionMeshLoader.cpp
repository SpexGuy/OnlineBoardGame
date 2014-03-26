#include <assert.h>
#include <sstream>
#include <string>
#include "CollisionMeshLoader.h"

using namespace std;

void trimString(string & str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}


vector<btVector3> *loadCollisionPoints(istream *stream) {
	istream &objStream = *stream;
	vector<btVector3> *retPoints = new vector<btVector3>();
	vector<btVector3> &points = *retPoints;

	assert(objStream);

	string line, token;

	getline(objStream, line);
	while (!objStream.eof()) {
		trimString(line);
		if (line.length() > 0 && line.at(0) != '#') {
			istringstream lineStream(line);

			lineStream >> token;

			if (token == "v") {
				float x, y, z;
				lineStream >> x >> y >> z;
				points.push_back(btVector3(x, y, z));
			}
		}
		getline(objStream, line);
	}

	return retPoints;
}
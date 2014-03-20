#include <assert.h>
#include <sstream>
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


bool loadCollisionMesh(istream *stream,
					   vector<btVector3> *outPoints,
					   vector<int> *outTrigs)
{
	istream &objStream = *stream;
	vector<btVector3> &points = *outPoints;
	vector<int> &trigs = *outTrigs;

	assert(objStream);
	assert(points.size() == 0);
	assert(trigs.size() == 0);

	string line, token;
	vector<int> face;

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
			} else if (token == "f") {
				// Process face
				face.clear();
				size_t slash1;
				while (lineStream.good()) {
					string vertString;
					lineStream >> vertString;
					int pIndex = -1;

					slash1 = vertString.find("/");
					if (slash1 == string::npos){
						pIndex = atoi(vertString.c_str()) - 1;
					} else {
						pIndex = atoi(vertString.substr(0, slash1).c_str()) - 1;
					}
					if (pIndex == -1) {
						printf("Missing point index!!!");
					} else {
						face.push_back(pIndex);
					}
				}
				int v0 = face[0];
				int v1 = face[1];
				int v2 = face[2];
				// First face
				trigs.push_back(v0);
				trigs.push_back(v1);
				trigs.push_back(v2);
				// If number of edges in face is greater than 3,
				// decompose into triangles as a triangle fan.
				for (unsigned int i = 3; i < face.size(); i++) {
					v1 = v2;
					v2 = face[i];
					trigs.push_back(v0);
					trigs.push_back(v1);
					trigs.push_back(v2);
				}
			}
		}
		getline(objStream, line);
	}

	return true;
}

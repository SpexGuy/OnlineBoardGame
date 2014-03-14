#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include "GLErrorCheck.h"
#include "GraphicsContext.h"
#include "GraphicsMesh.h"
#include "Vertex.h"

using namespace std;
using namespace glm;

void trimString(string & str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}


GraphicsMesh* GraphicsMesh::loadMesh(string fileName) {
	vector<vec3> points;
	vector<vec3> normals;
	vector<vec2> texCoords;
	vector<GLuint> faces;

	int nFaces = 0;

	bool loadTex = false;

	ifstream objStream(fileName, std::ios::in);

	if (!objStream) {
		cerr << "Unable to open OBJ file: " << fileName << endl;
		return NULL;
	}

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
				points.push_back(vec3(x, y, z));
			} else if (token == "vt" && loadTex) {
				// Process texture coordinate
				float s, t;
				lineStream >> s >> t;
				texCoords.push_back(vec2(s, t));
			} else if (token == "vn") {
				float x, y, z;
				lineStream >> x >> y >> z;
				normals.push_back(vec3(x, y, z));
			} else if (token == "f") {
				nFaces++;

				// Process face
				face.clear();
				size_t slash1, slash2;
				//int point, texCoord, normal;
				while (lineStream.good()) {
					string vertString;
					lineStream >> vertString;
					int pIndex = -1, nIndex = -1, tcIndex = -1;

					slash1 = vertString.find("/");
					if (slash1 == string::npos){
						pIndex = atoi(vertString.c_str()) - 1;
					} else {
						slash2 = vertString.find("/", slash1 + 1);
						pIndex = atoi(vertString.substr(0, slash1).c_str()) - 1;
						if (slash2 > slash1 + 1) {
							tcIndex = atoi(vertString.substr(slash1 + 1, slash2).c_str()) - 1;
						}
						nIndex = atoi(vertString.substr(slash2 + 1, vertString.length()).c_str()) - 1;
					}
					if (pIndex == -1) {
						printf("Missing point index!!!");
					} else {
						face.push_back(pIndex);
					}

					if (loadTex && tcIndex != -1 && pIndex != tcIndex) {
						printf("Texture and point indices are not consistent.\n");
					}
					if (nIndex != -1 && nIndex != pIndex) {
						printf("Normal and point indices are not consistent.\n");
					}
				}
				// If number of edges in face is greater than 3,
				// decompose into triangles as a triangle fan.
				if (face.size() > 3) {
					int v0 = face[0];
					int v1 = face[1];
					int v2 = face[2];
					// First face
					faces.push_back(v0);
					faces.push_back(v1);
					faces.push_back(v2);
					for (unsigned int i = 3; i < face.size(); i++) {
						v1 = v2;
						v2 = face[i];
						faces.push_back(v0);
						faces.push_back(v1);
						faces.push_back(v2);
					}
				} else {
					faces.push_back(face[0]);
					faces.push_back(face[1]);
					faces.push_back(face[2]);
				}
			}
		}
		getline(objStream, line);
	}

	objStream.close();
		
	cout << "Loaded mesh from: " << fileName << endl;

	vec3 dimensions = vec3(0.0f);

	vector<VertexPNT> vertices;

	for(unsigned int i = 0; i < points.size(); i++){
		vertices.push_back(VertexPNT(points[i], normals[i], vec2(0.0f, 0.0f)));
	}

	GraphicsMesh *mesh = new GraphicsMesh(vertices, faces);
	return mesh;
}






GraphicsMesh::GraphicsMesh(const vector<VertexPNT> &vertices, const vector<GLuint> &faces) {
	numElements = faces.size();
	
	//make a new VAO for this mesh
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);
	//transfer coordinate data to the graphics card
	glGenBuffers(1, &coordBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, coordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	//transfer element data to the graphics card
	glGenBuffers(1, &elementBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(faces[0]), &faces[0], GL_STATIC_DRAW);
	//set up shader inputs
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) (1*sizeof(vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (GLvoid *) (2*sizeof(vec3)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkError("After GraphicsMesh ctor");
}


void GraphicsMesh::draw(const mat4 &world) {
	GraphicsContext::inst()->
		drawTriangles(numElements, vertexArrayHandle, world);
}

GraphicsMesh::~GraphicsMesh() {
	glBindVertexArray(vertexArrayHandle);
	glDeleteBuffers(1, &coordBufferHandle);
	glDeleteBuffers(1, &elementBufferHandle);
	glDeleteVertexArrays(1, &vertexArrayHandle);
	checkError("After GraphicsMesh dtor");
}

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

void GraphicsMesh::trimString(string & str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}


GraphicsMesh* GraphicsMesh::loadMesh(istream *stream) {
	istream &objStream = *stream;
	int time = glutGet(GLUT_ELAPSED_TIME);
	vector<vec3> points;
	vector<vec3> normals;
	vector<vec2> texCoords;
	vector<VertexPNT> verts;
	int vertIndex = 0;
	vector<GLuint> faces;

	int nFaces = 0;

	bool loadTex = true;

	string line, token;
	vector<ivec3> face;

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
					} else if (tcIndex == -1) {
						printf("Missing texture index!!!");
					} else if (nIndex == -1) {
						printf("Missing normal index!!!");
					} else {
						face.push_back(ivec3(pIndex, tcIndex, nIndex));
					}
				}
				ivec3 v0 = face[0];
				ivec3 v1 = face[1];
				ivec3 v2 = face[2];
				int baseIndex = verts.size();
				// First face
				faces.push_back(baseIndex);
				verts.push_back(VertexPNT(points[v0.x], normals[v0.z], texCoords[v0.y]));
				faces.push_back(verts.size());
				verts.push_back(VertexPNT(points[v1.x], normals[v1.z], texCoords[v1.y]));
				faces.push_back(verts.size());
				verts.push_back(VertexPNT(points[v2.x], normals[v2.z], texCoords[v2.y]));
				// If number of edges in face is greater than 3,
				// decompose into triangles as a triangle fan.
				for (unsigned int i = 3; i < face.size(); i++) {
					v1 = v2;
					v2 = face[i];
					faces.push_back(baseIndex);
					faces.push_back(verts.size()-1);
					faces.push_back(verts.size());
					verts.push_back(VertexPNT(points[v2.x], normals[v2.z], texCoords[v2.y]));
				}
			}
		}
		getline(objStream, line);
	}

	cout << "Loaded mesh from " << objStream << " in " << glutGet(GLUT_ELAPSED_TIME) - time << "ms" << endl;

	GraphicsMesh *mesh = new GraphicsMesh(verts, faces);
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

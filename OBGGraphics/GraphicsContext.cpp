#include <assert.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "GLErrorCheck.h"
#include "GLSLProgram.h"
#include "GraphicsContext.h"


using namespace glm;
using namespace std;


GraphicsContext *instance;

void obgGraphicsInit(int argc, char *argv[]) {
	glutInit(&argc, argv);
	ilInit();
	iluInit();
	ilutInit();

	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("Test Window");

	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		assert(false);
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f,0.2f,0.2f,1.0f);

	instance = new GraphicsContext();

	cout << "SUCCESS!" << endl;
}


GraphicsContext *GraphicsContext::inst() {
	return instance;
}

void GraphicsContext::setupFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	viewport();
}

void GraphicsContext::viewport() {
	glViewport(0, 0, size.x, size.y);
}

void GraphicsContext::useShader(GLSLProgram *shader) {
	currShader = shader;
	shader->use();
}

void GraphicsContext::drawTriangles(GLsizei numElements, GLuint vertexArrayHandle,
							   const mat4 &world)
{
	glBindVertexArray(vertexArrayHandle);
	currShader->setup(world);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	checkError("After draw triangles");
}


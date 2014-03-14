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

	instance = new GraphicsContext();

	cout << "SUCCESS!" << endl;
}


GraphicsContext *GraphicsContext::inst() {
	return instance;
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


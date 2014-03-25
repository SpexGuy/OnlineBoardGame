#include <assert.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "GLErrorCheck.h"
#include "GLSLProgram.h"
#include "GraphicsContext.h"


using namespace glm;
using namespace std;


GraphicsContext *instance;

void OBGGraphicsInit(int argc, char *argv[]) {
	glutInit(&argc, argv);
	ilInit();
	iluInit();
	ilutInit();

	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
}

void OBGGraphicsCreateContext() {
	glutCreateWindow("Test Window");

	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		assert(false);
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f,0.2f,0.2f,1.0f);

	instance = new GraphicsContext();
}

GraphicsContext *GraphicsContext::inst() {
	return instance;
}

GraphicsContext::GraphicsContext() :
	overlayShader(new GLSLProgram()),
	currShader(NULL)
{
	overlayShader->compileShader("simpleShader.vert");
	overlayShader->compileShader("simpleShader.frag");
	overlayShader->link();
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

void GraphicsContext::drawText2D(float x, float y, const char *str, float size, const vec3 &color) const {
	glUseProgram(0);
	mat4 pos = translate(overlayBase, vec3(x, y, 0));
	float scaleFactor = size/72.0f;
	pos = scale(pos, vec3(scaleFactor, scaleFactor, 1.0f));
	//iterate through the string
	overlayShader->use();
	checkError("After use shader");
	overlayShader->setUniform("color", color);
	checkError("After set color");
	while(*str != '\0') {
		overlayShader->setUniform("mvp", pos);
		checkError("After set mvp");
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
		checkError("After stroke character");
		//translate past the character we just drew
		pos = translate(pos, vec3(glutStrokeWidth(GLUT_STROKE_ROMAN, *str), 0.0f, 0.0f));
		checkError("After glutStrokeWidth");
		str++;
	}
	//unbind the shader
	if (currShader != NULL)
		currShader->use();
	checkError("After draw text");
}

void GraphicsContext::setSize(int x, int y) {
	size = glm::ivec2(x,y);
	overlayBase = ortho(0.0f, float(x),
						0.0f, float(y),
						0.0f, 1.0f) *
				  lookAt(vec3(0.0f, 0.0f, 0.0f),
						 vec3(0.0f, 0.0f,-1.0f),
						 vec3(0.0f, 1.0f, 0.0f));
}

GraphicsContext::~GraphicsContext() {
	delete overlayShader;
}

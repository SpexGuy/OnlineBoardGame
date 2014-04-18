#include "GraphicsContext.h"
#include <assert.h>
#include <iostream>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "GLErrorCheck.h"
#include "GLSLProgram.h"


using namespace glm;
using namespace std;


GLFWwindow *buildWindow(bool fullscreen, GLFWwindow *context) {
	glfwWindowHint(GLFW_SAMPLES, 8);
	return glfwCreateWindow(1024, 512, "BoardNow", fullscreen ? glfwGetPrimaryMonitor() : NULL, context);
}

GraphicsContext *instance = NULL;

bool OBGGraphicsInit(int argc, char *argv[]) {
	glutInit(&argc, argv);
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		assert(false);
		return false;
	}
	ilInit();
	iluInit();
	ilutInit();
	return true;
}

bool OBGGraphicsCreateContext() {
	GLFWwindow *window = buildWindow(false, NULL);
	
	if (!window) {
		cerr << "Could not create window" << endl;
		assert(false);
		return false;
	}
	glfwMakeContextCurrent(window);
		
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		assert(false);
		return false;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.2f,0.2f,0.2f,1.0f);

	instance = new GraphicsContext(window);
	return true;
}

GraphicsContext *GraphicsContext::inst() {
	assert(instance);
	return instance;
}

GraphicsContext::GraphicsContext(GLFWwindow *window) :
	window(window),
	overlayShader(new GLSLProgram()),
	currShader(NULL)
{
	overlayShader->compileShader("simpleShader.vert");
	overlayShader->compileShader("simpleShader.frag");
	overlayShader->link();
}

//void GraphicsContext::rebuildWindow() {
//	GLFWwindow *newWindow = buildWindow(fullscreen, window);
//	if (newWindow) {
//		glfwDestroyWindow(window);
//		window = newWindow;
//	}
//}

//void GraphicsContext::setFullscreen(bool fullscreen) {
//	if (this->fullscreen != fullscreen) {
//		this->fullscreen = fullscreen;
//		rebuildWindow();
//	}
//}

//void GraphicsContext::toggleFullscreen() {
//	fullscreen = !fullscreen;
//	rebuildWindow();
//}

bool GraphicsContext::shouldCloseWindow() {
	return glfwWindowShouldClose(window) == GL_TRUE;
}

void GraphicsContext::setShouldCloseWindow(bool status) {
	glfwSetWindowShouldClose(window, status);
}

bool GraphicsContext::isVisible() {
	return glfwGetWindowAttrib(window, GLFW_VISIBLE) == GL_TRUE;
}

void GraphicsContext::setKeyCallback(GLFWkeyfun func) {
	glfwSetKeyCallback(window, func);
}

void GraphicsContext::setMouseCallback(GLFWmousebuttonfun func) {
	glfwSetMouseButtonCallback(window, func);
}

void GraphicsContext::setMouseMotionCallback(GLFWcursorposfun func) {
	glfwSetCursorPosCallback(window, func);
}

void GraphicsContext::pollEvents() {
	glfwPollEvents();
}

void GraphicsContext::setupFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	viewport();
}

void GraphicsContext::viewport() {
	ivec2 size = getFramebufferSize();
	glViewport(0, 0, size.x, size.y);
}

void GraphicsContext::useShader(GLSLProgram *shader) {
	currShader = shader;
}

void GraphicsContext::drawTriangles(GLsizei numElements, GLuint vertexArrayHandle,
									const mat4 &world)
{
	glBindVertexArray(vertexArrayHandle);
	currShader->use();
	currShader->setup(world);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	checkError("After draw triangles");
}

void GraphicsContext::drawText2D(float x, float y, const char *str, float size, const vec3 &color) const {
	ivec2 fbsize = getFramebufferSize();
	mat4 overlayBase =
		ortho(0.0f, float(fbsize.x),
			  0.0f, float(fbsize.y),
			  0.0f, 1.0f) *
		lookAt(vec3(0.0f, 0.0f, 0.0f),
			   vec3(0.0f, 0.0f,-1.0f),
			   vec3(0.0f, 1.0f, 0.0f));

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
	glUseProgram(0);
	checkError("After draw text");
}

void GraphicsContext::closeWindow() {
	glfwDestroyWindow(window);
	window = NULL;
}

ivec2 GraphicsContext::getWindowSize() const {
	ivec2 size;
	glfwGetWindowSize(window, &size.x, &size.y);
	return size;
}

ivec2 GraphicsContext::getFramebufferSize() const {
	ivec2 size;
	glfwGetFramebufferSize(window, &size.x, &size.y);
	return size;
}

void GraphicsContext::swapBuffers() {
	glfwSwapBuffers(window);
}

GraphicsContext::~GraphicsContext() {
	delete overlayShader;
}

#pragma once
#ifdef _WIN32
#	include <Windows.h>
#endif
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GLSLProgram;

bool OBGGraphicsInit(int argc, char *argv[]);

bool OBGGraphicsCreateContext();

class GraphicsContext {
private:
	GLSLProgram *currShader;
	glm::mat4 projection;
	glm::mat4 view;
	GLFWwindow *window;
	//bool fullscreen;

	GLSLProgram *overlayShader;

	//void rebuildWindow();

public:
	static GraphicsContext *inst();

	GraphicsContext(GLFWwindow *window);

	bool shouldCloseWindow();
	void setShouldCloseWindow(bool status);
	bool isVisible();
	void setKeyCallback(GLFWkeyfun func);
	void setMouseCallback(GLFWmousebuttonfun func);
	void setMouseMotionCallback(GLFWcursorposfun func);
	void pollEvents();
	void setupFrame();
	void swapBuffers();
	void viewport();
	void drawTriangles(GLsizei numElements, GLuint vertexArrayHandle,
					   const glm::mat4 &world);
	void useShader(GLSLProgram *shader);
	void drawText2D(float x, float y, const char *str, float size, const glm::vec3 &color) const;
	void closeWindow();
	glm::ivec2 getWindowSize() const;
	glm::ivec2 getFramebufferSize() const;
	//void setFullscreen(bool fullscreen);
	//void toggleFullscreen();
	~GraphicsContext();

	//Trivial Setters and Getters
	inline void setProjection(const glm::mat4 &proj) { projection = proj; }
	inline glm::mat4 getProjection() { return projection; }
	inline void setView(const glm::mat4 &v) { view = v; }
	inline glm::mat4 getView() { return view; }
	//inline bool isFullscreen() { return fullscreen; }
};

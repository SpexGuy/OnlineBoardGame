#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class GLSLProgram;

void obgGraphicsInit(int argc, char *argv[]);

class GraphicsContext {
private:
	GLSLProgram *currShader;
	glm::mat4 projection;
	glm::mat4 view;
	glm::ivec2 size; 

public:
	static GraphicsContext *inst();


	void drawTriangles(GLsizei numElements, GLuint vertexArrayHandle,
					   const glm::mat4 &world);

	void useShader(GLSLProgram *shader);

	//Trivial Setters and Getters
	inline void setProjection(const glm::mat4 &proj) { projection = proj; }
	inline glm::mat4 getProjection() { return projection; }
	inline void setView(const glm::mat4 &v) { view = v; }
	inline glm::mat4 getView() { return view; }
	inline void setSize(int x, int y) { size = glm::ivec2(x,y); }
	inline glm::ivec2 getSize() { return size; }
};

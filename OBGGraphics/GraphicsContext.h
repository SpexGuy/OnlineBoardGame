#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class GLSLProgram;

void OBGGraphicsInit(int argc, char *argv[]);

void OBGGraphicsCreateContext();

class GraphicsContext {
private:
	GLSLProgram *currShader;
	glm::mat4 projection;
	glm::mat4 view;
	glm::ivec2 size;

	GLSLProgram *overlayShader;
	glm::mat4 overlayBase;

public:
	static GraphicsContext *inst();

	GraphicsContext();

	void setupFrame();
	void viewport();
	void drawTriangles(GLsizei numElements, GLuint vertexArrayHandle,
					   const glm::mat4 &world);
	void useShader(GLSLProgram *shader);
	void drawText2D(float x, float y, const char *str, float size, const glm::vec3 &color) const;
	void setSize(int x, int y);
	virtual ~GraphicsContext();

	//Trivial Setters and Getters
	inline void setProjection(const glm::mat4 &proj) { projection = proj; }
	inline glm::mat4 getProjection() { return projection; }
	inline void setView(const glm::mat4 &v) { view = v; }
	inline glm::mat4 getView() { return view; }
	//nontrivial size setter; see above
	inline glm::ivec2 getSize() { return size; }
};

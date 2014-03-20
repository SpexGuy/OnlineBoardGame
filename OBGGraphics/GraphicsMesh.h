#pragma once
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "Drawable.h"

#define BAD_GL_VALUE GLuint(-1)

class VertexPNT;

class GraphicsMesh :
	public Drawable
{
protected:
	GLuint vertexArrayHandle;
	GLuint coordBufferHandle;
	GLuint elementBufferHandle;
	GLsizei numElements;
public:

	static GraphicsMesh *loadMesh(std::istream *source);

	GraphicsMesh(const std::vector<VertexPNT> &verts,
				 const std::vector<GLuint> &trigs);

	virtual void draw(const glm::mat4 &world);
	virtual ~GraphicsMesh();
};

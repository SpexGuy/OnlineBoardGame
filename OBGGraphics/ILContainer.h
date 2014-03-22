/* This class was written by Perry Kivolowitz */
#pragma once
#undef	_UNICODE
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#define	BAD_IL_VALUE	IL_INVALID_VALUE
#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

/** This class represents an image loaded using DevIL
 * which is used as a texture */
class ILContainer {
private:
	ILContainer();

	const char *filename;
	ILint width;
	ILint height;
	ILint format;
	ILuint il_handle;
	GLuint il_texture_handle;
	GLvoid *data;
public:
	ILContainer(const char *filename);

	/* binds the texture to its channel */
	virtual void bind(int channel);
	virtual ~ILContainer();
};

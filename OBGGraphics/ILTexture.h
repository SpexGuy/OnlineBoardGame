/* This class was written by Perry Kivolowitz */
#pragma once
#undef	_UNICODE
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "Texture.h"

#define	BAD_IL_VALUE	IL_INVALID_VALUE
#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

/** This class represents an image loaded using DevIL
 * which is used as a texture */
class ILTexture :
	public Texture
{
private:
	ILTexture();

	const char *filename;
	int channel;
	ILint width;
	ILint height;
	ILint format;
	ILuint il_handle;
	GLuint il_texture_handle;
	GLvoid *data;
public:
	ILTexture(const char *filename, int channel) {
		this->filename = filename;
		this->il_handle = BAD_IL_VALUE;
		this->il_texture_handle = BAD_GL_VALUE;
		this->channel = channel;
	}

	/* loads the texture and registers it with GL */
	virtual bool initialize();
	/* binds the texture to its channel */
	virtual void bind();
	/* releases any GL resources */
	virtual void takeDown();

	virtual ~ILTexture();
};

/* This class was written by Perry Kivolowitz */
#include <assert.h>
#include <iostream>
#include "ILContainer.h"


ILContainer::ILContainer(const char *filename) {
	this->filename = filename;
}

bool ILContainer::initialize() {
	if ((this->il_handle = ilGenImage()) == BAD_IL_VALUE)
		return false;
	ilBindImage(this->il_handle);
	if (!ilLoadImage(filename))
		return false;

	glGenTextures(1, &this->il_texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	format = ilGetInteger(IL_IMAGE_FORMAT);
	data = ilGetData();
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), width, height, 0, format, GL_UNSIGNED_BYTE, data);

	return true;
}

void ILContainer::bind(int channel) {
	// We are asserting that we have initialized this object before.
	assert(this->il_handle != BAD_IL_VALUE);

	glActiveTexture(GL_TEXTURE0 + channel);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
}

ILContainer::~ILContainer() {
	//assert that takeDown has been called
	glDeleteTextures(1, &this->il_texture_handle);
	ilDeleteImage(this->il_handle);
}

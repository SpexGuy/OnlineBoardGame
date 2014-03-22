/* This class was written by Perry Kivolowitz */
#include <assert.h>
#include "ILContainer.h"


ILContainer::ILContainer(const char *filename) {
	this->filename = filename;

	if ((this->il_handle = ilGenImage()) == BAD_IL_VALUE)
		assert(false);
	ilBindImage(this->il_handle);
	if (!ilLoadImage(filename))
		assert(false);

	glGenTextures(1, &this->il_texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), this->width = ilGetInteger(IL_IMAGE_WIDTH), this->height = ilGetInteger(IL_IMAGE_HEIGHT), 0, this->format = ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, this->data = ilGetData());
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

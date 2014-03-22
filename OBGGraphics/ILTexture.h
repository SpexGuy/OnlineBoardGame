#pragma once
#include "Texture.h"

class ILContainer;

class ILTexture :
	public Texture
{
protected:
	ILContainer *image;
	int channel;
public:
	ILTexture(ILContainer *texture, int channel) :
		image(texture), channel(channel) {}

	virtual void bind();

	virtual ~ILTexture() {}
};

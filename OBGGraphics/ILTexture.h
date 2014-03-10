#pragma once
#include <string>
#include "Texture.h"

class ILTexture :
	public Texture
{
public:
	ILTexture(std::string filename);
	virtual void bind();
	virtual ~ILTexture();
};

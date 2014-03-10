#pragma once
#include "ShaderMaterial.h"

class Texture;

class TextureMaterial {
protected:
	Texture *tex;

public:
	virtual void bind();
	virtual ~TextureMaterial();
};

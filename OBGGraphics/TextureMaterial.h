#pragma once
#include "ShaderMaterial.h"

class Texture;

class TextureMaterial :
	public ShaderMaterial
{
protected:
	Texture *tex;

public:
	TextureMaterial(Texture *tex, GLSLProgram *shader) :
		ShaderMaterial(shader), tex(tex) {}
	virtual void bind();
	virtual ~TextureMaterial() {}
};

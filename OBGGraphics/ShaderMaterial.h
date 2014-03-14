#pragma once
#include "Material.h"

class GLSLProgram;

class ShaderMaterial :
	public Material
{
protected:
	GLSLProgram *shader;

public:
	ShaderMaterial(GLSLProgram *shader) :
		shader(shader) {}
	virtual void bind();
	virtual ~ShaderMaterial() {}
};

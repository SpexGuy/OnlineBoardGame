#pragma once
#include "Material.h"

class GLSLProgram;

class ShaderMaterial {
protected:
	GLSLProgram *shader;

public:
	virtual void bind();
	virtual ~ShaderMaterial();
};

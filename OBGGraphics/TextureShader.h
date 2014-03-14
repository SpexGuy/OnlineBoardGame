#pragma once
#include "MatrixShader.h"

class TextureShader :
	public MatrixShader
{
	virtual void setup(const glm::mat4 &model);
	virtual ~TextureShader() {}
};

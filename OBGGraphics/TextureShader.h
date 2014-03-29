#pragma once
#include "MatrixShader.h"

class TextureShader :
	public MatrixShader
{
public:
	virtual void setup(const glm::mat4 &model);
	virtual ~TextureShader() {}
};

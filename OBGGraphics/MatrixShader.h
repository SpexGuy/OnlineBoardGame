#pragma once
#include "GLSLProgram.h"

class MatrixShader : public GLSLProgram {
public:
	void setup(const glm::mat4 &model);
	virtual ~MatrixShader() {}
};

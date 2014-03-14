#include "TextureShader.h"

using namespace glm;

void TextureShader::setup(const mat4 &model) {
	MatrixShader::setup(model);
	setUniform("colorTex", 0);
}
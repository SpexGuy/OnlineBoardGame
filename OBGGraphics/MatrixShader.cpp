#include <glm/glm.hpp>
#include "GraphicsContext.h"
#include "MatrixShader.h"

using namespace glm;

void MatrixShader::setup(const mat4 &model) {
	mat4 mv = GraphicsContext::inst()->getView() * model;
	this->setUniform("m", model);
	this->setUniform("mv", mv);
	this->setUniform("mvp", GraphicsContext::inst()->getProjection() * mv);
	this->setUniform("norm", inverse(transpose(mat3(mv))));
}

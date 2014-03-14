#include <glm/glm.hpp>
#include "GraphicsContext.h"
#include "MatrixShader.h"

using namespace glm;

void MatrixShader::setup(const mat4 &model) {
	this->setUniform("mvp",
		GraphicsContext::inst()->getProjection() *
		GraphicsContext::inst()->getView() *
		model);
	this->setUniform("m", model);
	this->setUniform("v", GraphicsContext::inst()->getView());
	this->setUniform("p", GraphicsContext::inst()->getProjection());
}

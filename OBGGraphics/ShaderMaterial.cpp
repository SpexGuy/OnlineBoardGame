#include "GraphicsContext.h"
#include "ShaderMaterial.h"

void ShaderMaterial::bind() {
	GraphicsContext::inst()->useShader(shader);
}
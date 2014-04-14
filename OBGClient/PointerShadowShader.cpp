#include <GraphicsContext.h>
#include "GameManager.h"
#include "MousePointer.h"
#include "PointerShadowShader.h"
#include "UserInputHandler.h"

void PointerShadowShader::setup(const mat4 &world) {
	TextureShader::setup(world);
	setUniform("pointerPos", GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos());
	setUniform("lightEyePos", GraphicsContext::inst()->getView() * vec4(0,50,0,0));
}

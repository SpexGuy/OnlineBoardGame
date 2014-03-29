#include "GameManager.h"
#include "MousePointer.h"
#include "PointerShadowShader.h"
#include "UserInputHandler.h"

void PointerShadowShader::setup(const mat4 &world) {
	TextureShader::setup(world);
	setUniform("pointerPos", GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos());
}

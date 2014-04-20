#include <GraphicsContext.h>
#include "GameManager.h"
#include "GLErrorCheck.h"
#include "MousePointer.h"
#include "PointerShadowShader.h"
#include "UserInputHandler.h"

void PointerShadowShader::setup(const mat4 &world) {
	checkError("Before setup PSShader");
	TextureShader::setup(world);
	checkError("After setup TShader");
	setUniform("pointerPos", GameManager::inst()->getInputHandler()->getMousePointer()->getWorldPos());
	checkError("Set pointer uniform");
	setUniform("lightEyePos", vec3(GraphicsContext::inst()->getView() * vec4(0,50,0,1)));
	checkError("Set light uniform");
}

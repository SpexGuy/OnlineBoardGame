#pragma once
#include <TextureShader.h>

class PointerShadowShader :
	public TextureShader
{
public:
	virtual void setup(const glm::mat4 &world);
};

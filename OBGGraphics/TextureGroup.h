#pragma once
#include <vector>
#include "Texture.h"

class TextureGroup :
	public Texture
{
protected:
	std::vector<Texture *> textures;

public:

	virtual void bind();

	/* returns this, for chaining */
	TextureGroup *addTexture(Texture *tex);

	virtual ~TextureGroup();
};

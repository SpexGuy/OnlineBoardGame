#include "Texture.h"
#include "TextureGroup.h"



TextureGroup *TextureGroup::addTexture(Texture *tex) {
	textures.push_back(tex);
	return this;
}

void TextureGroup::bind() {
	for (int c = 0; c < textures.size(); c++) {
		textures[c]->bind();
	}
}

TextureGroup::~TextureGroup() {
	//textures are freed by the entity which allocated them
}

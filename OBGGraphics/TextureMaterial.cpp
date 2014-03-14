#include "Texture.h"
#include "TextureMaterial.h"


void TextureMaterial::bind() {
	tex->bind();
	ShaderMaterial::bind();
}

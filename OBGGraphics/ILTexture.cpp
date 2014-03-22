#include "ILContainer.h"
#include "ILTexture.h"

void ILTexture::bind() {
	image->bind(channel);
}

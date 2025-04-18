#include "pch.h"
#include "Texture.h"

Texture::Texture(std::string path) {
	if (texture.loadFromFile(path)) {
		sprite.setTexture(texture);
	}
}
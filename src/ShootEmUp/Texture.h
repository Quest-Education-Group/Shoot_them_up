#pragma once
#include<string>
#include "SFML/Graphics/Texture.hpp"

class Texture
{
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Texture(std::string path);
	sf::Sprite& GetTexture() { return sprite; };
};
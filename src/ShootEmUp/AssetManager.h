#pragma once
#include<map>
class Texture;
class AssetManager
{
	std::map<std::string, Texture*> mTextures;
public:
	static AssetManager* Get();

	Texture* LoadTexture(std::string path);
	Texture* GetTexture(std::string path);
};
#include "pch.h"
#include "AssetManager.h"
#include "Texture.h"
AssetManager* AssetManager::Get()
{
	static AssetManager mInstance;

	return &mInstance;
}

Texture* AssetManager::LoadTexture(std::string path) {
	Texture* pTexture = new Texture(path);
	mTextures[path] = pTexture;
	std::cout << "image load: " << path << std::endl;
	return pTexture;

}
Texture* AssetManager::GetTexture(std::string path) {
	auto it = mTextures.find(path);

	if (it == mTextures.end())
		return LoadTexture(path);

	return it->second;
}
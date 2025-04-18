#include "Background.h"

#include "GameManager.h"

template<typename T>
T* Background::GetScene() const
{
	T* pScene = dynamic_cast<T*>(GetScene());

	_ASSERT(pScene != nullptr);

	return pScene;
}


template<typename T>
T* Background::CreateParalax(int height, int speed, int layer, std::string path)
{
	return GetScene()->CreateParalax<T>(height, speed, layer, path);
}

template<typename T>
T* Background::CreateBackground(float width, float height, std::string path)
{
	return GetScene()->CreateBackground<T>(width, height, path);
}

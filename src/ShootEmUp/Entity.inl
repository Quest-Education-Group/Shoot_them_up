#include "Entity.h"

#include "GameManager.h"

template<typename T>
T* Entity::GetScene() const
{
	T* pScene = dynamic_cast<T*>(GetScene());

	_ASSERT(pScene != nullptr);

	return pScene;
}


template<typename T>
T* Entity::CreateEntity(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path)
{
	return GetScene()->CreateEntity<T>(nbAnimation, scale, tag, id, path);
}
#include "UI.h"

#include "GameManager.h"

template<typename T>
T* UI::GetScene() const
{
	T* pScene = dynamic_cast<T*>(GetScene());

	_ASSERT(pScene != nullptr);

	return pScene;
}


template<typename T>
T* UI::CreateUI(int scaleX, int scaleY, sf::String tag, std::string path)
{
	return GetScene()->CreateBackground<T>(scaleX, scaleY, tag, path);
}

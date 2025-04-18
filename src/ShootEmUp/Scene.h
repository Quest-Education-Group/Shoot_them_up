#pragma once

class GameManager;

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
class Scene
{
	GameManager* mpGameManager;
	void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	GameManager* GetGameManager() { return mpGameManager; }

	
protected:
	Scene() = default;

	virtual void OnInitialize() = 0;
	virtual void OnEvent(const sf::Event& event) = 0;
	virtual void OnUpdate() = 0;

public:

	template<typename T>
	T* CreateEntity(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path);

	template<typename T>
	T* CreateParalax(int height, int speed, int layer, std::string path);

	template<typename T>
	T* CreateBackground(float width, float height, std::string path);

	template<typename T>
	T* CreateUI(int scaleX, int scaleY, sf::String tag, std::string path);

	template<typename T>
	void LoadScene();

	float GetDeltaTime() const;

	int GetWindowWidth() const;
	int GetWindowHeight() const;

	float AdaptSize(float originalSize);

	friend GameManager;


};

#include "Scene.inl"
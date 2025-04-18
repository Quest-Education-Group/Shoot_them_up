#pragma once

#include <list>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

#define FIXED_DT 0.01667f
class Entity;
class UI;
class Background;
class Scene;
class Debug;

namespace sf
{
	class RenderWindow;
	class Event;
}

class GameManager
{
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	std::list<UI*> mUIs;
	std::list<UI*> mUIsToDestroy;
	std::list<UI*> mUIsToAdd;

	std::list<std::pair<Background*, Background*>> mParalaxs;
	std::list<std::pair<Background*, Background*>> mParalaxsToDestroy;
	std::list<std::pair<Background*, Background*>> mParalaxsToAdd;

	std::list<Background*> mBackgrounds;
	std::list<Background*> mBackgroundsToDestroy;
	std::list<Background*> mBackgroundsToAdd;

	sf::RenderWindow* mpWindow;
	sf::Font mFont;

	Scene* mpScene;

	float mDeltaTime;
	float mAccumulatedDt = 0.f;

	int mWindowWidth;
	int mWindowHeight;

	bool mHaveBackground;
	bool mHavePlayer;
	bool mInPause = false;
	bool mExit = false;

private:

	void Run();

	void HandleInput();
	void Update();
	void FixeUpdate();
	void Draw();
	void ChangeScene();



public:
	GameManager();
	~GameManager();
	static GameManager* Get();
	std::list<Entity*>& GetEntities() { return mEntities; };

	void CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit = 60);


	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }

	Scene* GetScene() const { return mpScene; }
	sf::Font& GetFont() { return mFont; };
	sf::RenderWindow* GetWindow() const { return mpWindow; }

	void SetInPause(bool value) { mInPause = value; }
	bool GetInPause() const { return mInPause; }

	void SetExit(bool value) { mExit = value; }
	bool GetExit() const { return mExit; }

	void SetHaveBackgroud(bool value) { mHaveBackground = value; }
	void SetHavePlayer(bool value) { mHavePlayer = value; }

	bool GetHaveBackgroud() const { return mHaveBackground; }
	bool GetHavePlayer() const { return mHavePlayer; }

	friend Debug;
	friend Scene;
};

#include "GameManager.inl"
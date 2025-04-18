#include "pch.h"
#include "GameManager.h"

#include "Entity.h"
#include "Background.h"
#include "UI.h"
#include "Debug.h"


#include <iostream>

GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
	mpWindow->setFramerateLimit(fpsLimit);

	mWindowWidth = width;
	mWindowHeight = height;
}

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("../../../res/Hack-Regular.ttf");
	_ASSERT(fontLoaded);

	_ASSERT(mpScene != nullptr);

	sf::Clock clock;
	while (mpWindow->isOpen())
	{
		SetDeltaTime(clock.restart().asSeconds());

		HandleInput();

		Update();

		Draw();
	}
}

void GameManager::HandleInput()
{
	sf::Event event;
	while (mpWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || GetExit() == true)
		{
			mpWindow->close();
		}

		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
			SetInPause(!GetInPause());
		}

		mpScene->OnEvent(event);
	}
}

void GameManager::FixeUpdate() {
	// Entity
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		Entity* entity = *it;

		entity->FixeUpdate(FIXED_DT);

		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mEntitiesToDestroy.push_back(entity);
		it = mEntities.erase(it);
	}
	//Collision
	for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
	{
		auto it2 = it1;
		++it2;
		for (; it2 != mEntities.end(); ++it2)
		{
			Entity* entity = *it1;
			Entity* otherEntity = *it2;

			if (entity->IsColliding(otherEntity))
			{
				entity->OnCollision(otherEntity);
				otherEntity->OnCollision(entity);
			}
		}
	}

	// Entity
	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	mEntitiesToDestroy.clear();

	// Entity
	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::Update()
{
	if (mInPause)
		return;

	mpScene->OnUpdate();

	//Update
	mAccumulatedDt += mDeltaTime;
	while (mAccumulatedDt >= FIXED_DT) {
		FixeUpdate();
		mAccumulatedDt -= FIXED_DT;

	}

	// Paralax
	for (auto it = mParalaxs.begin(); it != mParalaxs.end(); )
	{
		std::pair<Background*, Background*> Paralax = *it;

		Paralax.first->Update();
		Paralax.second->Update();

		if (Paralax.first->ToDestroy() == false && Paralax.second->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mParalaxsToDestroy.push_back(Paralax);
		it = mParalaxs.erase(it);
	}

	for (auto it = mParalaxsToDestroy.begin(); it != mParalaxsToDestroy.end(); ++it)
	{
		delete it->first;
		delete it->second;
	}

	mParalaxsToDestroy.clear();

	for (auto it = mParalaxsToAdd.begin(); it != mParalaxsToAdd.end(); ++it)
	{
		mParalaxs.push_back(*it);
		mParalaxs.sort([](const std::pair<Background*, Background*>& a,const std::pair<Background*, Background*>& b)
			{
				return a.first->mlayer < b.first->mlayer;
			});
	}

	mParalaxsToAdd.clear();

	// Backgrounds
	for (auto it = mBackgrounds.begin(); it != mBackgrounds.end(); )
	{
		Background* backgroud = *it;

		backgroud->Update();

		if (backgroud->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mBackgroundsToDestroy.push_back(backgroud);
		it = mBackgrounds.erase(it);
	}

	for (auto it = mBackgroundsToDestroy.begin(); it != mBackgroundsToDestroy.end(); ++it)
	{
		delete* it;
	}

	mBackgroundsToDestroy.clear();

	for (auto it = mBackgroundsToAdd.begin(); it != mBackgroundsToAdd.end(); ++it)
	{
		mBackgrounds.push_back(*it);
	}

	mBackgroundsToAdd.clear();
	

	// UI
	for (auto it = mUIs.begin(); it != mUIs.end(); )
	{
		UI* ui = *it;

		ui->Update();

		if (ui->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mUIsToDestroy.push_back(ui);
		it = mUIs.erase(it);
	}

	for (auto it = mUIsToDestroy.begin(); it != mUIsToDestroy.end(); ++it)
	{
		delete* it;
	}

	mUIsToDestroy.clear();

	for (auto it = mUIsToAdd.begin(); it != mUIsToAdd.end(); ++it)
	{
		mUIs.push_back(*it);
	}

	mUIsToAdd.clear();

}

void GameManager::Draw()
{
	mpWindow->clear();

	// Paralax
	for (auto& Paralax : mParalaxs)
	{
		mpWindow->draw(Paralax.first->GetSprite());
		mpWindow->draw(Paralax.second->GetSprite());
	}

	// Background 
	for (Background* backgroud : mBackgrounds)
	{
		mpWindow->draw(backgroud->GetSprite());
	}

	// Entity
	for (Entity* entity : mEntities)
	{
		mpWindow->draw(entity->GetSprite());
	}

	// UI
	for (UI* ui : mUIs)
	{
		mpWindow->draw(ui->GetSprite());
	}

	Debug::Get()->Draw(mpWindow);

	mpWindow->display();
}



void GameManager::ChangeScene() {
	// Paralax
	for (auto it = mParalaxs.begin(); it != mParalaxs.end(); )
	{
		std::pair<Background*, Background*> backgroundpair = *it;

		mParalaxsToDestroy.push_back(backgroundpair);
		it = mParalaxs.erase(it);
	}

	//Background
	for (auto it = mBackgrounds.begin(); it != mBackgrounds.end(); )
	{
		Background* background = *it;

		mBackgroundsToDestroy.push_back(background);
		it = mBackgrounds.erase(it);
	}

	// Entity
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		Entity* entity = *it;
		mEntitiesToDestroy.push_back(entity);
		it = mEntities.erase(it);
	}

	//UI
	for (auto it = mUIs.begin(); it != mUIs.end(); )
	{
		UI* ui = *it;

		mUIsToDestroy.push_back(ui);
		it = mUIs.erase(it);
	}


	mpScene = nullptr;
}

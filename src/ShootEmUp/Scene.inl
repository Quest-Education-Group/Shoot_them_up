#pragma once
#include "pch.h"
#include "Scene.h"
#include "GameManager.h"
#include "Background.h"

template<typename T>
T* Scene::CreateEntity(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	float adaptSize = AdaptSize(scale);

	Entity* entity = newEntity;
	entity->Initialize(nbAnimation, adaptSize, tag, id, path);

	mpGameManager->mEntitiesToAdd.push_back(newEntity);

	return newEntity;
}

template<typename T>
T* Scene::CreateParalax(int height, int speed, int layer, std::string path)
{
	static_assert(std::is_base_of<Background, T>::value, "T must be derived from Background");

	T* newParalax1 = new T();
	T* newParalax2 = new T();

	newParalax1->Initialize(height, speed, layer, 1, path);
	newParalax2->Initialize(height, speed, layer, 2, path);

	std::pair<Background*, Background*> Paralax = { newParalax1, newParalax2 };

	mpGameManager->mParalaxsToAdd.push_back(Paralax);

	return newParalax1;
}

template<typename T>
T* Scene::CreateBackground(float width, float height, std::string path)
{
	static_assert(std::is_base_of<Background, T>::value, "T must be derived from Background");

	T* newBackgroud1 = new T();

	newBackgroud1->InitializeBackground(width, height, path);

	mpGameManager->mBackgroundsToAdd.push_back(newBackgroud1);

	return newBackgroud1;
}

template<typename T>
T* Scene::CreateUI(int scaleX, int scaleY, sf::String tag, std::string path)
{
	static_assert(std::is_base_of<UI, T>::value, "T must be derived from UI");

	T* newUI = new T();

	UI* ui = newUI;
	ui->Initialize(scaleX, scaleY, tag, path);
	mpGameManager->mUIsToAdd.push_back(newUI);

	return newUI;
}




template<typename T>
void Scene::LoadScene()
{
	GameManager::Get()->LaunchScene<T>();
}
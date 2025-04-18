#pragma once

#include "Scene.h"

class MoteurEntity;
class Background;
class Entity;
class Ship;
class MoteurUI;

class SampleScene : public Scene
{
	Entity* pEntity1;
	Entity* pEntity2;
	Entity* pEntity3;
	MoteurUI* pUi;
	Ship* player;

	Background* pBackground1;
	Background* pBackground2;

	Entity* pEntitySelected;

private:
	void TrySetSelectedEntity(Entity* pEntity, int x, int y);

public:
	static SampleScene* Get();
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};
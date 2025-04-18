#pragma once

#include "Scene.h"

class MoteurEntity;
class Background;
class MoteurUI;
class Ship;

class SceneMoteur : public Scene
{
	MoteurEntity* pEntity1;
	MoteurEntity* pEntity2;
	MoteurEntity* pEntity3;
	Ship* player;
	MoteurUI* pUi;

	Background* pBackground1;
	Background* pBackground2;

	MoteurEntity* pEntitySelected;

private:
	void TrySetSelectedEntity(MoteurEntity* pEntity, int x, int y);

public:
	static SceneMoteur* Get();
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};



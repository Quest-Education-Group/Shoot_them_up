#pragma once
#include <string>
#include "Scene.h"


class Enemy;
class GameplayScene;

class LevelManager : public Scene
{
	Enemy* pEntity1;
	Enemy* pEntity2;

private:
	char a;
	const std::string lien = "../../../src/shootemup/Test.txt";
	std::string ligne;


public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void Read();
};



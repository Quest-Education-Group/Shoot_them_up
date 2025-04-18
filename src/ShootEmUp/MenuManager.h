#pragma once

#include "Scene.h"
#include <vector>
#include <string>
#include <functional>

class Background;
class Entity;

class MenuManager : public Scene
{
protected:
    int finalScore;

    Background* pBackground1;
    Background* pBackground2;
    std::vector<Entity*> buttons;
    std::vector<std::function<void()>> buttonActions;

    void InitializeBackground(const std::string& bgPath1, const std::string& bgPath2 = "");
    void InitializeButton(int index, const char* buttonPath, float xOffset, float yOffset);

public:
    void OnInitialize() override {};
    void OnEvent(const sf::Event& event) override;
    void OnUpdate() override {}

	void SetFinalScore(int score);
};

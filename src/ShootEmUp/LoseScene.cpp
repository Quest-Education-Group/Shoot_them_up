#include "pch.h"
#include "LoseScene.h"

#include "GameplayScene.h"
#include "Debug.h"

LoseScene* LoseScene::Get() {
    static LoseScene instance;
    return &instance;
}

void LoseScene::OnInitialize() {
    InitializeBackground("../../../res/Menu_Lose.png", "../../../res/Defaite.png");

    InitializeButton(0, "../../../res/COMMENCER_LA_PARTIE", 75, 4 * (GetWindowHeight() / 7));
    buttonActions.push_back([this]() {
        this->LoadScene<GameplayScene>();
        });

    InitializeButton(1, "../../../res/REPRENDRE_LA_PARTIE", 75, 5 * (GetWindowHeight() / 7));    
    buttonActions.push_back([]() {
        std::cout << "Lvl Choice" << std::endl;
        });

    InitializeButton(2, "../../../res/QUITTER_LE_JEU", 75, 6 * (GetWindowHeight() / 7));
    buttonActions.push_back([]() {
        GameManager::Get()->SetExit(true);
        });
}

void LoseScene::OnUpdate() {
    //DrawScore();
}


void LoseScene::DrawScore() {
    sf::Text tempText;
    tempText.setFont(GameManager::Get()->GetFont());
    tempText.setString("Votre score: " + std::to_string(finalScore));
    tempText.setCharacterSize(20);
    const sf::FloatRect bounds = tempText.getLocalBounds();

    float x = GetWindowWidth() / 2.f - bounds.width / 2.f;
    float y = GetWindowHeight() / 3.f;

    Debug::DrawText(x, y, "Votre score: " + std::to_string(finalScore), sf::Color::Black);
}

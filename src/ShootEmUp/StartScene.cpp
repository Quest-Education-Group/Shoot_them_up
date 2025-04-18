#include "pch.h"
#include "StartScene.h"

#include "Entity.h"
#include "AllScene.h"

StartScene* StartScene::Get()
{
	static StartScene mInstance;

	return &mInstance;
}



void StartScene::OnInitialize() {
    InitializeBackground("../../../res/Menu_Start.png");

    InitializeButton(0, "../../../res/COMMENCER_LA_PARTIE", 75, 3 * (GetWindowHeight() / 7));
    buttonActions.push_back([this]() {
        this->LoadScene<GameplayScene>();
        });

    InitializeButton(1, "../../../res/PARAMETRES", 75, 4 * (GetWindowHeight() / 7));
    buttonActions.push_back([]() {
        std::cout << "Parametres" << std::endl;
        });

    InitializeButton(2, "../../../res/QUITTER_LE_JEU", 75, 5 * (GetWindowHeight() / 7));
    buttonActions.push_back([]() {
        GameManager::Get()->SetExit(true);
        });
}

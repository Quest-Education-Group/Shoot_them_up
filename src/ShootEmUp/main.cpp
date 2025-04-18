#include "pch.h"
#include <iostream>

#include "GameManager.h"
#include "AllScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{

	GameManager* pInstance = GameManager::Get();

	int screen = 1;
	switch (screen)
	{
	case 0:
		pInstance->CreateWindow(1080, 720, "SceneManager");
		break;
	case 1:
		pInstance->CreateWindow(1920, 1080, "SceneManager");
		break;
	case 2:
		pInstance->CreateWindow(2560, 1440, "SceneManager");
		break;
	case 3:
		pInstance->CreateWindow(3840, 2160, "SceneManager");
		break;
	}


	int scene = 3;
	switch (scene)
	{
	case 0:
		pInstance->LaunchScene<SampleScene>();
		break;
	case 1:
		pInstance->LaunchScene<SceneMoteur>();
		break;
	case 2:
		pInstance->LaunchScene<GameplayScene>();
		break;
	case 3:
		pInstance->LaunchScene<StartScene>();
		break;
	case 4:
		pInstance->LaunchScene<WinScene>();
		break;
	case 5:
		pInstance->LaunchScene<LoseScene>();
		break;
	case 6 :
		/*pInstance->LaunchScene<PauseScene>();*/
		break;
	}

	return 0;
}
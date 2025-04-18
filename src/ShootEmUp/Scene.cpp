#include "pch.h"
#include "Scene.h"

#include "GameManager.h"
#include "Background.h"
#include "Entity.h"

int Scene::GetWindowWidth() const
{
	return mpGameManager->mWindowWidth;
}

int Scene::GetWindowHeight() const
{
	return mpGameManager->mWindowHeight;
}

float Scene::GetDeltaTime() const
{
	return mpGameManager->mDeltaTime;
}

float Scene::AdaptSize(float originalSize) {
    float scaleX = GetWindowWidth() / 1920.0f;
    float scaleY = GetWindowHeight() / 1080.0f;

    float scale = std::min(scaleX, scaleY);

    scale = std::sqrt(scale);

    return originalSize * scale;
}



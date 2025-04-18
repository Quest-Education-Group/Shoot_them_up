#include "pch.h"
#include "SampleScene.h"

#include "TestMoteurEntity.h"
#include "Ship.h"
#include "Entity.h"
#include "AllScene.h"

#include "Debug.h"

SampleScene* SampleScene::Get()
{
	static SampleScene mInstance;

	return &mInstance;
}

void SampleScene::OnInitialize()
{
	pBackground1 = CreateParalax<Background>(30, 25.f, 2, "../../../res/paralax1.png");
	pBackground2 = CreateParalax<Background>(380, 50.f, 1, "../../../res/route.png");

	player = CreateEntity<Ship>(5, 0.2f, "Entity", "Player", "../../../res/Explosion");
	player->SetTypeEntityCollider(Entity::eCercle);
	player->SetHP(80);
	player->SetPosition(500, 150);

	pEntity2 = CreateEntity<Entity>(5, 0.5f, "Entity", "Enemy", "../../../res/Explosion");
	pEntity2->SetTypeEntityCollider(Entity::eAABB);
	pEntity2->SetPosition(500, 150);


	pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event& event)
{

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		//TrySetSelectedEntity(pEntity1, event.mouseButton.x, event.mouseButton.y);
		TrySetSelectedEntity(pEntity2, event.mouseButton.x, event.mouseButton.y);
		//TrySetSelectedEntity(pEntity3, event.mouseButton.x, event.mouseButton.y);
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected != nullptr)
		{
			pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
		}
	}

}

void SampleScene::TrySetSelectedEntity(Entity* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 10, sf::Color::Blue);
	}

	if (pEntity2 == nullptr) {
		LoadScene<LoseScene>();
	}

	Debug::DrawLine(0, 0, GetWindowWidth(), GetWindowHeight(), sf::Color::Red);
}

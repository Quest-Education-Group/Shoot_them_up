#include "pch.h"
#include "SceneMoteur.h"

#include "TestMoteurEntity.h"
#include "AllScene.h"
#include "Background.h"

#include "Debug.h"
#include "Ship.h"

SceneMoteur* SceneMoteur::Get()
{
	static SceneMoteur mInstance;

	return &mInstance;
}


void SceneMoteur::OnInitialize()
{
	if (GameManager::Get()->GetHaveBackgroud() == false) {
		pBackground2 = CreateParalax<Background>(150, 25.f, 1, "../../../res/back2.png");
		pBackground1 = CreateParalax<Background>(-100, 50.f, 2, "../../../res/back1.png");
	}

	if (GameManager::Get()->GetHavePlayer() == false) {
		player = CreateEntity<Ship>(1, 1, "Entity", "Player", "../../../res/spritesheet");
		player->SetTypeEntityCollider(Entity::eCercle);
		player->SetHP(80);
		player->SetPosition(100, 100);
	}


	pUi = CreateUI<MoteurUI>(64*3, 64, "pause", "../../../res/spritesheet.jpg");
	pUi->SetPosition(400, 150);

	pEntity2 = CreateEntity<MoteurEntity>(1, 1, "Entity", "e", "../../../res/spritesheet");
	pEntity2->SetTypeEntityCollider(Entity::eAABB);
	pEntity2->SetPosition(800, 100);


}

void SceneMoteur::OnEvent(const sf::Event& event)
{
	

	//if (event.mouseButton.button == sf::Mouse::Button::Right)
	//{
	//	//TrySetSelectedEntity(pEntity1, event.mouseButton.x, event.mouseButton.y);
	//	TrySetSelectedEntity(pEntity2, event.mouseButton.x, event.mouseButton.y);
	//	ActionUI(pUi, event.mouseButton.x, event.mouseButton.y);
	//}

	//if (event.mouseButton.button == sf::Mouse::Button::Left)
	//{


	//	/*if (pEntitySelected != nullptr && pEntitySelected->GetTypeCollider() != Entity::eClickBox)
	//	{
	//		pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
	//	}
	//	else if (pEntitySelected != nullptr && pEntitySelected->GetTypeCollider() == Entity::eClickBox)
	//	{
	//		LoadScene<SampleScene>();
	//	}*/
	//}
}

void SceneMoteur::TrySetSelectedEntity(MoteurEntity* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SceneMoteur::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 10, sf::Color::Blue);
	}
}
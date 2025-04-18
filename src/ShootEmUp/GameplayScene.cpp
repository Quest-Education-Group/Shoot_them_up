#include "pch.h"
#include "GameplayScene.h"
#include "LevelManager.h"
#include "Ship.h"
#include "Enemy.h"
#include "AllScene.h"
#include "UI.h"

#include <fstream>
#include "Debug.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "Debug.h"

int GameplayScene::nextLevel = 1;

GameplayScene::GameplayScene() {
	levelFiles = {
		{0, "../../../src/shootemup/lvl_Debug.txt"},
		{1, "../../../src/shootemup/lvl_1.txt"},
		{2, "../../../src/shootemup/lvl_2.txt"},
		{3, "../../../src/shootemup/lvl_3.txt"},
		{4, "../../../src/shootemup/lvl_4.txt"},
		{5, "../../../src/shootemup/lvl_5.txt"},
	};
}


void GameplayScene::OnInitialize()
{
	pBackground1 = CreateParalax<Background>(0, 10.f, 1, "../../../res/ciel.png");
	pBackground2 = CreateParalax<Background>(0, 20.f, 2, "../../../res/plan2.png");
	pBackground3 = CreateParalax<Background>(0, 30.f, 3, "../../../res/plan1.png");
	pBackground4 = CreateParalax<Background>(GetWindowHeight() / 2, 50.f, 4, "../../../res/route.png");

	player = CreateEntity<Ship>(6, 0.15f, "Entity", "Player", "../../../res/Voiture");
	player->SetTypeEntityCollider(Entity::eCercle);
	player->SetHP(100);
	player->SetPosition(GetWindowWidth() / 10, GetWindowHeight() / 2);

	 
	fire = CreateEntity<Entity>(1, 1, "Entity", "Bullet", "../../../res/fire1"); 
	fire->SetTypeEntityCollider(Entity::eAABB);
	fire->SetPosition(-50, -50);

	pEntitySelected = nullptr;

	if (nextLevel > levelFiles.size() - 1) {
		DestroyAllEntities();
		LoadScene<StartScene>();
	}
	else {
		LoadLevel(nextLevel);
	}
}

void GameplayScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;
}

void GameplayScene::TrySetSelectedEntity(Ship* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void GameplayScene::OnUpdate()
{
	static const float waveInterval = 0.8f;

	if (!isGameOver) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
			fire->SetPosition(-50, -50);
		}

		player->Move(GetDeltaTime());

		Debug::DrawText(15.f, 15.f, "Score: " + std::to_string(mScore), sf::Color::Green);

		DrawHP(player);

		if (waveClock.getElapsedTime().asSeconds() >= waveInterval) {
			SpawnWave();
			waveClock.restart();
		}

		DestroyBullet();
		DestroyEnemy();

		LoseCondition();

		DebugShoot();

		for (auto* bullet : pBullets) {
			if (bullet != nullptr) {
				int bulletType = bullet->GetBulletType();
				bullet->BulletManager(this, bullet, bulletType);
			}
		}

		for (auto* enemy : pEnemies) {
			if (enemy != nullptr) {
				int enemyType = enemy->GetEnemyType();
				enemy->EnemyManager(this, enemy, enemyType);
			}
		}

	}
}

void GameplayScene::DestroyBullet() {
	for (auto it = pBullets.begin(); it != pBullets.end();)
	{
		Bullet* pBullet = *it;
		
		//if (pBullet->GetLifeTime() >= 5) {
		//	pBullet->Destroy();
		//	it = pBullets.erase(it);
		//}
		if (pBullet->OutOfScreen() || pBullet->ToDestroy())
		{
			if (pBullet->GetCanGoOut() == true) {
				return;
			}
			else {
				/*std::cout << "Bullet destroyed" << std::endl;*/
				pBullet->Destroy();
				it = pBullets.erase(it);
			}
		}
		else
		{
			++it;
		}
	}
}


void GameplayScene::DestroyEnemy() {
	for (auto it = pEnemies.begin(); it != pEnemies.end(); )
	{
		Enemy* pEnemy = *it;
		if (pEnemy->OutOfScreen() || pEnemy->ToDestroy())
		{
			/*std::cout << "Enemy destroyed" << std::endl;*/
			pEnemy->Destroy();
			it = pEnemies.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameplayScene::AddEnemy(Enemy* enemy) {
	if (enemy) {
		pEnemies.push_back(enemy);
	}
}


void GameplayScene::AddBullet(Bullet* bullet) {
	if (bullet) {
		pBullets.push_back(bullet);

	}
}


Enemy* GameplayScene::GetClosestEnemy(sf::Vector2f position) {
	Enemy* closestEnemy = nullptr;
	float closestDistance = std::numeric_limits<float>::max();

	for (Enemy* enemy : pEnemies) {
		float distance = std::sqrt(
			std::pow(enemy->GetPosition().x - position.x, 2) +
			std::pow(enemy->GetPosition().y - position.y, 2)
		);

		if (distance < closestDistance) {
			closestDistance = distance;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;
}

sf::Vector2f GameplayScene::GetShipPos() {
	return player->GetPosition();
}



void GameplayScene::SpawnEnemy(int EnemyType, int line /*, const EnemyParams param */)
{
	float scale;

	/*if (param) {
		scale = param->size / 2.0f;
	}
	else {
		if (EnemyType == 4) {
			scale = 0.55f;
		}
		else {
			scale = 0.15f;
		}
	} */

	if (EnemyType == 4) {
		scale = 1.5f;
	}
	else {
		scale = 0.5f;
	}

	Enemy* pEnemy = CreateEntity<Enemy>(5, scale, "Entity", "Enemy", "../../../res/ENNEMI_BASIQUE_MECHA");

	pEnemy->SetTypeEntityCollider(Entity::eAABB);

	pEnemy->EnemyCreator(this, pEnemy, EnemyType, sf::Vector2f(-1, 0), 5 + line/*, scale*/);
	m_spawnClock.restart();
}

void GameplayScene::SpawnBullet(int mBulletType, sf::Vector2f direction, sf::String mTeam, sf::Vector2f mPos)
{
	sf::String bulletTag;
	if (mTeam == "left") {
		direction = -(direction);
		bulletTag = "EnemyBullet";
	}
	else {
		bulletTag = "PlayerBullet";
	}
	Bullet* pBullet = CreateEntity<Bullet>(15, 0.2f, "Entity", bulletTag, "../../../res/Missile");
	pBullet->SetTypeEntityCollider(Entity::eAABB);



	pBullet->BulletCreator(this, pBullet, mBulletType, direction, mPos);
	m_spawnClock.restart();
}

void GameplayScene::DestroyAllEntities() {
	for (auto* bullet : pBullets) {
		bullet->Destroy();
	}
	pBullets.clear();

	for (auto* enemy : pEnemies) {
		enemy->Destroy();
	}
	pEnemies.clear();

	player->Destroy();

	/*std::cout << "All Entities Destroyed" << std::endl;*/
}

void GameplayScene::LoseCondition() {
	if (player->GetHP() <= 0) {
		DestroyAllEntities();

		isGameOver = true;

		/*std::cout << "You lost! -> Ship and all entities destroyed <-" << std::endl;*/
		DestroyAllEntities();

		LoseScene* loseScene = dynamic_cast<LoseScene*>(LoseScene::Get());
		if (loseScene) {
			loseScene->SetFinalScore(mScore);
		}

		LoadScene<LoseScene>();
		return;
	}
}

void GameplayScene::WinCondition() {
	if (pEnemies.empty() && !isGameOver) {
		/*std::cout << "You won! -> All enemies destroyed <-" << std::endl;*/
		DestroyAllEntities();

		WinScene* winScene = dynamic_cast<WinScene*>(WinScene::Get());
		if (winScene) {
			winScene->SetFinalScore(mScore);
		}
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		std::cout << "Niveau " << nextLevel << " termine avec un score de " << mScore << std::endl;
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		LoadScene<WinScene>();
	}
}

void ShootSound() {
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	if (!buffer->loadFromFile("../../../res/laser-shot-sound.wav")) {
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		std::cerr << "Erreur de chargement du fichier audio!" << std::endl;
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		return;
	}
	/*std::cout << "Fichier charge avec succes\n";*/
	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*buffer);
	sound->play();
}

void GameplayScene::DebugShoot() {
	Bullet* pBullet = nullptr;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 2, 1); ShootSound();
			player->SetIsShooting(true);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 0, 9); ShootSound();
			player->SetIsShooting(true);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 0, 24); ShootSound();
			player->SetIsShooting(true);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 1, 1); ShootSound();
			player->SetIsShooting(true);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 1, 5); ShootSound();
			player->SetIsShooting(true);
		}
	}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {

		if (shootClock.getElapsedTime().asSeconds() >= 0.1f) { // Intervalle entre les tirs
			pBullet->Shoot(this, player->GetPosition(), "right", 3, 1); 
			shootClock.restart();
		}
	}


	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && laser_spawnClock.getElapsedTime().asSeconds() >= 0.5f) {
		if (!player->IsShooting()) {
			pBullet->Shoot(this, player->GetPosition(), "right", 4, 1); 
			player->SetIsShooting(true);
		}
		laser_spawnClock.restart();
	}
   	else {
		player->SetIsShooting(false);
	}
}


std::vector<Enemy*> GameplayScene::GetEnemies() {
	return pEnemies;
}

std::vector<Bullet*> GameplayScene::GetBullets() {
	return pBullets;
}

void GameplayScene::ReadAndInitialize(std::ifstream& lien) {
	if (lien.is_open()) {
		/*std::cout << "Fichier ouvert" << std::endl;*/

		std::string ligne;
		enemyData.clear(); 

		while (std::getline(lien, ligne)) {
			std::vector<int> row;
			for (char c : ligne) {
				if (c != ' ' && c != '-') {
					row.push_back(c - '0'); 
				}
				else {
					row.push_back(-1);
				}
			}
			enemyData.push_back(row);
		}

		if (enemyData.empty()) {
			std::cerr << "ERREUR: Le fichier est vide ou mal formate." << std::endl;
		}
	}
	else {
		std::cerr << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
	}
}

void GameplayScene::LoadLevel(int level) {
	if (levelFiles.find(level) != levelFiles.end()) {
		currentLevel = level;
		const std::string& filePath = levelFiles[level];

		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		std::cout << "Chargement du niveau " << level << " depuis " << filePath << std::endl;
		std::cout << "-----------------------------------------------------------------------------" << std::endl;

		std::ifstream monFlux(filePath);
		if (monFlux.is_open()) {
			/*std::cout << "Fichier ouvert avec succes : " << filePath << std::endl;*/
			waveCount = 0;
			ReadAndInitialize(monFlux); 
			monFlux.close(); 
		}
		else {
			std::cout << "-----------------------------------------------------------------------------" << std::endl;
			std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
			std::cout << "-----------------------------------------------------------------------------" << std::endl;
		}
	}
	else {
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		std::cerr << "Erreur : Niveau " << level << " non defini dans levelFiles" << std::endl;
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
	}
}


void GameplayScene::NextLevel() {
	int nextLevel = currentLevel + 1;
	if (levelFiles.find(nextLevel) != levelFiles.end()) {
		LoadLevel(nextLevel);
	}
	else {
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
		std::cout << "Aucun niveau suivant defini. Fin du jeu !" << std::endl;
		std::cout << "-----------------------------------------------------------------------------" << std::endl;
	}
}



void GameplayScene::SpawnEnemiesFromData(size_t frameCount) {
	size_t column = frameCount % enemyData[0].size();
	//std::cout << "Spawn enemies for column " << column + 1 << std::endl;

	for (size_t row = 0; row < enemyData.size(); ++row) {
		if (column < enemyData[row].size() && enemyData[row][column] != -1) {
			int enemyType = enemyData[row][column];
			SpawnEnemy(enemyType, static_cast<int>(row + 1));
			/*std::cout << "Un ennemi pop (type " << enemyType << ", ligne " << row + 1 << ")" << std::endl;*/
		}
	}
}


void GameplayScene::SpawnWave() {
	if (waveCount < enemyData[0].size()) {
		SpawnEnemiesFromData(waveCount);
		waveCount++;
		/*std::cout << "Score actuel : " << mScore << std::endl;*/
	}
	else
	{
		WinCondition();
	}
}



void GameplayScene::SetNextLevel(int level) {
	nextLevel = level;
}

int GameplayScene::GetNextLevel() {
	return nextLevel;
}

void GameplayScene::AddScore(int value) {
	mScore += value;
	UpdateScoreText();
}

int GameplayScene::GetScore() const {
	return mScore;
}

void GameplayScene::UpdateScoreText()
{
	scoreText.setString("Score: " + std::to_string(mScore));
}

void GameplayScene::DrawHP(Ship* player) {
	if (player != nullptr) {
		std::string hpText = std::to_string(player->GetHP()) + "/100 HP";
		sf::Text sfText;
		sfText.setFont(GameManager::Get()->GetFont());
		sfText.setString(hpText);
		sfText.setCharacterSize(20);

		const sf::FloatRect bounds = sfText.getLocalBounds();

		Debug::DrawText(
			GetWindowWidth() - bounds.width - 15.f, 
			GetWindowHeight() - bounds.height - 15.f, 
			hpText,
			sf::Color::Green
		);
	}
}
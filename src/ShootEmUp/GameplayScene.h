#pragma once

#include <thread>
#include <atomic>
#include "Scene.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

class Ship;
class Enemy;
class UI;
class Background;
class Bullet;

class GameplayScene : public Scene
{
	Ship* player;
	Enemy* pEnemy;
	UI* pUi;

	Background* pBackground1;
	Background* pBackground2;
	Background* pBackground3;
	Background* pBackground4;

	Entity* pEntity2;
	Entity* fire;
	Entity* laser;

	std::vector<Enemy*> pEnemies;
	std::vector<Bullet*> pBullets; 

	sf::Clock m_spawnClock;
	sf::Clock laser_spawnClock;
	Ship* pEntitySelected;

private:
	void TrySetSelectedEntity(Ship* pEntity, int x, int y);

	bool isGameOver = false;
	sf::Text scoreText;
	int mScore = 0;
	char a;

	std::map<int, std::string> levelFiles; 
	int currentLevel = 1;
	static int nextLevel;

	std::string ligne;

	std::thread m_readThread; 
	std::atomic<bool> m_running;

	sf::Clock shootClock;
	sf::Clock waveClock; 
	size_t waveCount = 0; 

	std::vector<std::vector<int>> enemyData;

public:
	GameplayScene();

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	void LoseCondition();
	void WinCondition();

	void ReadAndInitialize(std::ifstream& lien);
	void SpawnEnemiesFromData(size_t frameCount);
	void SpawnWave(); 
	void LoadLevel(int level);
	void NextLevel();
	static void SetNextLevel(int level);
	static int GetNextLevel();

	void SpawnEnemy(int EnemyType, int line);
	void SpawnBullet(int BulletType, sf::Vector2f direction, sf::String NewDir, sf::Vector2f mPos);
	void DebugShoot();

	void AddScore(int value);
	int GetScore() const;
	void UpdateScoreText();
	void DrawScore(sf::Font font, sf::Text scoreText);

	sf::Vector2f GetShipPos();

	std::vector<Enemy*> GetEnemies();
	std::vector<Bullet*> GetBullets();
	void AddEnemy(Enemy* enemy);
	void AddBullet(Bullet* bullet);
	void DestroyEnemy();
	void DestroyBullet();
	void DestroyAllEntities();

	void DrawHP(Ship* player);


	Enemy* GetClosestEnemy(sf::Vector2f position);
};



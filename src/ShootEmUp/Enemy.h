#pragma once
#include "Entity.h"
#include "GameplayScene.h"

class Enemy : public Entity
{
public:
	// EnemyParams GetParams() const;
	// void SetParams(const EnemyParams& params);

	void Move(float deltaTime);

	void OnCollision(Entity* other);
	bool OutOfScreen();

	void SetHP(int hp);
	int GetHP() const;

	void SetScoreValue(int scoreValue);
	int GetScoreValue() const;


	void SetSize(int size);
	int GetSize() const;

	void SetDamage(int hp);
	int GetDamage() const;

	void SetCanGoOut(bool canGoOut);
	bool GetCanGoOut() const;

	void SetEnemyType(int bulletType);
	int GetEnemyType() const;

	void SetUpdateTimer(int threshold);
	bool ShouldUpdate();

	virtual void EnemyCreator(GameplayScene* scene, Enemy* pEnemy, int EnemyType, sf::Vector2f direction, int line);
	virtual void EnemyManager(GameplayScene* scene, Enemy* Enemy, int EnemyType);

private:
	int mHP;
	int mDamage;
	int mEnemyType;
	float mSize;
	int mScoreValue;

	int mDivisionsRemaining = 0;
	bool mCanGoOut;

	int mUpdateCounter = 0;
	int mUpdateTimer;
};


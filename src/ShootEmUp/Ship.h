#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameplayScene.h"
#include "GameManager.h"

class Ship : public Entity
{
public:
	void OnCollision(Entity* other) override;
	void Move(float deltaTime);

	void SetHP(int hp);
	int GetHP() const;

	bool IsShooting() const;
	void SetIsShooting(bool firing);



private:

	bool mIsFiring = false;
	int mHP;
};



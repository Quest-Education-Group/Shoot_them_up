#include "pch.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>	

void Enemy::OnCollision(Entity* other)
{
    if (other->GetId() == "PlayerBullet") {
        Bullet* bullet = dynamic_cast<Bullet*>(other);
        if (bullet) {
            this->SetHP(this->GetHP() - bullet->GetDamage());
            if (GetHP() <= 0) {
                GameplayScene* scene = dynamic_cast<GameplayScene*>(this->GetScene());
                if (scene) {
                    scene->AddScore(this->GetScoreValue());
                    //if (mDivisionsRemaining > 0) {
                    //    GameplayScene* scene = dynamic_cast<GameplayScene*>(this->GetScene());
                    //    if (scene) {
                    //        EnemyParams params = this->GetParams();
                    //        params.hp /= 2;
                    //        params.damage /= 2;
                    //        params.scoreValue /= 2;
                    //        params.divisionsRemaining -= 1;
                    //        params.size /= 2;

                    //        for (int i = 0; i < 2; ++i) {
                    //            params.position.x += (i == 0) ? -50 : 50; // L?g?re s?paration des nouvelles entit?s
                    //            scene->SpawnEnemy(5, 3, &params); // Division
                    //        }
                    //    }
                    //}

                }
                this->Destroy();
            }
        }
    }
    else if (other->GetId() == "Player") {
        GameplayScene* scene = dynamic_cast<GameplayScene*>(this->GetScene());
        if (scene) {
            scene->AddScore(this->GetScoreValue() / 2);
        }
        this->Destroy();
    }
}


void Enemy::SetHP(int hp)
{
	mHP = hp;
}

int Enemy::GetHP() const
{
	return mHP;
}

void Enemy::SetScoreValue(int scoreValue)
{
    mScoreValue = scoreValue;
}

int Enemy::GetScoreValue() const
{
    return mScoreValue;
}

bool Enemy::OutOfScreen()
{
	return GetPosition().x < -GetSize() /*|| GetPosition().x > GetScene()->GetWindowWidth() + GetRadius()*/ ||
		GetPosition().y < -GetSize() || GetPosition().y > GetScene()->GetWindowHeight() + GetSize();
}

void Enemy::SetCanGoOut(bool canGoOut)
{
	mCanGoOut = canGoOut;
}

bool Enemy::GetCanGoOut() const
{
	return mCanGoOut;
}

void Enemy::EnemyCreator(GameplayScene* scene, Enemy* pEnemy, int EnemyType, sf::Vector2f direction, int line/*, float scale*/) {
    switch (EnemyType) {
    case 0://Basic enemy
        pEnemy->SetEnemyType(0);
        pEnemy->SetHP(5);
        pEnemy->SetDamage(1);
        pEnemy->SetSpeed(200);
        pEnemy->SetUpdateTimer(0);
        pEnemy->SetCanGoOut(false);
        pEnemy->SetScoreValue(100);
        //pEnemy->SetDivisionsRemaining(2);
        //pEnemy->SetSize(scale);
        break;

    case 1://Fast enemy
        pEnemy->SetEnemyType(1);
        pEnemy->SetHP(2);
        pEnemy->SetDamage(1);
        pEnemy->SetSpeed(750);
        pEnemy->SetUpdateTimer(0);
        pEnemy->SetCanGoOut(false);
        pEnemy->SetScoreValue(75);
        //pEnemy->SetSize(scale);
        break;

    case 2://Shooting enemy
        pEnemy->SetEnemyType(2);
        pEnemy->SetHP(3);
        pEnemy->SetDamage(1);
        pEnemy->SetSpeed(250);
        pEnemy->SetUpdateTimer(40);
        pEnemy->SetCanGoOut(false);
        pEnemy->SetScoreValue(150);
        //pEnemy->SetSize(scale);
        break;

    case 3: //Kamikaze enemy
        pEnemy->SetEnemyType(3);
        pEnemy->SetHP(1);
        pEnemy->SetDamage(3);
        pEnemy->SetSpeed(400);
        pEnemy->SetUpdateTimer(10);
        pEnemy->SetCanGoOut(false);
        pEnemy->SetScoreValue(200);
        //pEnemy->SetSize(scale);
        break;

    case 4: //Boss
        pEnemy->SetEnemyType(4);
        pEnemy->SetHP(100);
        pEnemy->SetDamage(1000);
        pEnemy->SetSpeed(100);
        pEnemy->SetUpdateTimer(10);
        pEnemy->SetCanGoOut(false);
        pEnemy->SetScoreValue(1000);
        //pEnemy->SetSize(scale);
        break;

        //  case 5: //Division
        //      pEnemy->SetEnemyType(4);
        //      pEnemy->SetHP(this->GetHP() / 2);
        //      pEnemy->SetDamage(this->GetDamage() / 2);
        //      pEnemy->SetSpeed(this->GetSpeed());
        //      pEnemy->SetUpdateTimer(this->GetUpdateTimer());
        //      pEnemy->SetCanGoOut(this->GetCanGoOut());
        //      pEnemy->SetScoreValue(this->GetScoreValue() / 2);
        //      pEnemy->SetDivisionsRemaining(mDivisionsRemaining - 1);
        //      pEnemy->SetSize(scale);
              //break;
    }

    pEnemy->SetTag("Enemy");
    pEnemy->SetPosition(scene->GetWindowWidth() + pEnemy->GetSize() * 2, (line * (scene->GetWindowHeight() / 11)) - (scene->GetWindowHeight() / 11) / 2);
    pEnemy->SetDirection(direction.x, direction.y, pEnemy->GetSpeed());

    if (pEnemy) {
        scene->AddEnemy(pEnemy);
    }
}

void Enemy::EnemyManager(GameplayScene* scene, Enemy* pEnemy, int EnemyType) {
    if (!pEnemy) {
        return;
    }

    sf::Vector2f direction = pEnemy->GetDirection();

    switch (EnemyType) {
	case 0: //Basic enemy
        pEnemy->SetDirection(direction.x, direction.y, pEnemy->GetSpeed());
        break;

	case 1: //Fast enemy
		pEnemy->SetDirection(direction.x, direction.y, pEnemy->GetSpeed());
		break;

	case 2: //Shooting enemy
        if (pEnemy->ShouldUpdate()) {
            sf::Vector2f position = pEnemy->GetPosition();
            sf::Vector2f direction = pEnemy->GetDirection();
            float speed = pEnemy->GetSpeed();

            if (position.x < scene->GetWindowWidth() / 2.0f) {
                direction.x = 1.0f;
            }
            else if (position.x > scene->GetWindowWidth() - pEnemy->GetSize()) {
                direction.x = -1.0f;
            }

            if (position.y < 0) {
                direction.y = 1.0f;
            }
            else if (position.y > scene->GetWindowHeight() - pEnemy->GetSize()) {
                direction.y = -1.0f;
            }

            if (pEnemy->GetPosition().x > 0 && pEnemy->GetPosition().x < scene->GetWindowWidth() && pEnemy->GetPosition().y > 0 && pEnemy->GetPosition().y < scene->GetWindowHeight()) {
                if (rand() % 100 < 40) {
                    Bullet* pBullet = nullptr;
                    pBullet->Shoot(scene, pEnemy->GetPosition(), "left", 0, 1);
                }
            }

            pEnemy->SetDirection(direction.x, direction.y, speed);
            pEnemy->Move(scene->GetDeltaTime());
        }
        break;

	case 3: //Kamikaze
        if (pEnemy->ShouldUpdate()) {
            sf::Vector2f targetPosition = scene->GetShipPos();
            sf::Vector2f directionShip = targetPosition - this->GetPosition();

            float magnitude = std::sqrt(directionShip.x * directionShip.x + directionShip.y * directionShip.y);
            if (magnitude != 0) {
                directionShip /= magnitude;
            }
            pEnemy->SetDirection(directionShip.x, directionShip.y, pEnemy->GetSpeed());
        }
        break;

    case 4: //Boss
        if (pEnemy->ShouldUpdate()) {
            sf::Vector2f position = pEnemy->GetPosition();
            sf::Vector2f direction = pEnemy->GetDirection();
            float speed = pEnemy->GetSpeed();

            if (position.x < 2 * (scene->GetWindowWidth() / 3.0f)) {
                direction.x = 1.0f;
            }
            else if (position.x > scene->GetWindowWidth() - pEnemy->GetSize()) {
                direction.x = -1.0f; 
            }

            if (position.y < 0) {
                direction.y = 1.0f; 
            }
            else if (position.y > scene->GetWindowHeight() - pEnemy->GetSize()) {
                direction.y = -1.0f; 
            }


            if (rand() % 100 < 20) { 
                Bullet* pBullet = nullptr;
                int randPattern = (rand() % 2) * 2 + 1;
                pBullet->Shoot(scene, pEnemy->GetPosition(), "left", 0, randPattern); 
                scene->AddBullet(pBullet);
            }

            pEnemy->SetDirection(direction.x, direction.y, speed);
            pEnemy->Move(scene->GetDeltaTime());
        }
		break;

    }

}

void Enemy::Move(float deltaTime) {
    if (GetEnemyType() == 4) { 
        sf::Vector2f position = GetPosition();
        sf::Vector2f direction = GetDirection();
        const float speed = GetSpeed();

        if (position.x < GetScene()->GetWindowWidth() / 2.0f) {
            direction.x = 1.0f;
        }
        else if (position.x > GetScene()->GetWindowWidth() - GetSize()) {
            direction.x = -1.0f; 
        }

        if (position.y < 0) {
            direction.y = 1.0f; 
        }
        else if (position.y > GetScene()->GetWindowHeight() - GetSize()) {
            direction.y = -1.0f; 
        }

        SetPosition(position.x + direction.x * speed * deltaTime, position.y + direction.y * speed * deltaTime);
        SetDirection(direction.x, direction.y, speed);
    }
}


void Enemy::SetUpdateTimer(int timer) {
    mUpdateTimer = timer;
}

bool Enemy::ShouldUpdate() {
    mUpdateCounter++;
    if (mUpdateCounter >= mUpdateTimer) {
        mUpdateCounter = 0;
        return true;
    }
    return false;
}

void Enemy::SetEnemyType(int enemyType)
{
    mEnemyType = enemyType;
}

int Enemy::GetEnemyType() const
{
    return mEnemyType;
}

void Enemy::SetDamage(int dmg)
{
    mDamage = dmg;
}

int Enemy::GetDamage() const
{
    return mDamage;
}

void Enemy::SetSize(int size) {
	mSize = size;
	//this->SetScale(size, size);
}

int Enemy::GetSize() const {
	return mSize;
}
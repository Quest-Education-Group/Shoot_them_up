#include "pch.h"
#include "Bullet.h"

#include "Debug.h"

#include <iostream>

void Bullet::OnCollision(Entity* other)
{
	if (other->GetId() == "Enemy" && this->GetId() == "PlayerBullet" && this->GetIsDestroyable()) {
		this->Destroy();
		/*std::cout << "Bullet destroyed" << std::endl;*/
	}
	else if (other->GetId() == "Player" && this->GetId() == "EnemyBullet") {
		this->Destroy();
		/*std::cout << "Bullet destroyed" << std::endl;*/

	}
}

void Bullet::SetDamage(int dmg)
{
	mDamage = dmg;
}

int Bullet::GetDamage() const
{
	return mDamage;
}

void Bullet::SetBulletType(int bulletType)
{
	mBulletType = bulletType;
}

int Bullet::GetBulletType() const
{
	return mBulletType;
}

bool Bullet::GetIsDestroyable()
{
	return mIsDestroyable;
}

void Bullet::SetIsDestroyable(bool isDestroyable)
{
	mIsDestroyable = isDestroyable;
}

void Bullet::SetStartPos(float x, float y) {
    mStartPosition = sf::Vector2f(x, y); 
}


sf::Vector2f Bullet::GetStartPos() const {
	return mStartPosition;
}

bool Bullet::OutOfScreen() {
	return GetPosition().x < -GetSize() || GetPosition().x > GetScene()->GetWindowWidth() + GetSize() ||
		GetPosition().y < -GetSize() || GetPosition().y > GetScene()->GetWindowHeight() + GetSize();
}

void Bullet::SetCanGoOut(bool canGoOut) {
	mCanGoOut = canGoOut;
}

bool Bullet::GetCanGoOut() const {
	return mCanGoOut;
}

void Bullet::SetUpdateTimer(int timer) {
	mUpdateTimer = timer;
}

bool Bullet::ShouldUpdate() {
	mUpdateCounter++;
	if (mUpdateCounter >= mUpdateTimer) {
		mUpdateCounter = 0; 
		return true;        
	}
	return false;           
}

void Bullet::Shoot(GameplayScene* scene, sf::Vector2f mPos, sf::String mTeam, int bulletType, int bulletNb) {
	float angleOffset = 0.0f;
    if (bulletType == 3) {
        bulletNb = 5;
        angleOffset = 8.0f;
    }
    else if (bulletType == 4) {
		bulletNb = 1;
    }
    else {
        angleOffset = 15.0f;
    }

    float baseAngle = 0.0f;
    float* angles = new float[bulletNb];

    int middleIndex = bulletNb / 2;
    for (int i = 0; i < bulletNb; ++i) {
        angles[i] = baseAngle + (i - middleIndex) * angleOffset;
    }

    for (int i = 0; i < bulletNb; ++i) {
        float angleRad = angles[i] * (PI / 180.0f);
        sf::Vector2f direction(std::cos(angleRad), std::sin(angleRad));

        scene->SpawnBullet(bulletType, direction, mTeam, mPos);
    }
    delete[] angles;
};


void Bullet::BulletCreator(GameplayScene* scene, Bullet* mBullet, int bulletType, sf::Vector2f direction, sf::Vector2f mPos) {

    switch (bulletType) {
    case 0: //Classic bullet
        mBullet->SetBulletType(0);
        mBullet->SetDamage(2);
        mBullet->SetSpeed(400);
        mBullet->SetUpdateTimer(0);
        mBullet->SetCanGoOut(false);
		mBullet->SetIsDestroyable(true);
        break;

    case 1: //Follow bullet
        mBullet->SetBulletType(1);
        mBullet->SetDamage(1);
        mBullet->SetSpeed(400);
        mBullet->SetUpdateTimer(10);
        mBullet->SetCanGoOut(false);
        mBullet->SetIsDestroyable(true);
        break;

    case 2: //Boomerang bullet
        mBullet->SetBulletType(2);
        mBullet->SetDamage(2);
        mBullet->SetSpeed(400);
        mBullet->SetUpdateTimer(10);
        mBullet->SetCanGoOut(false);
        mBullet->SetIsDestroyable(false);
        break;

	case 3: //Flame bullet
        mBullet->SetBulletType(3);
        mBullet->SetDamage(5); // Dommages moderes
        mBullet->SetSpeed(700); // Vitesse moderee
        mBullet->SetUpdateTimer(10);
        mBullet->SetCanGoOut(true);
        mBullet->SetIsDestroyable(false);
        break;

    case 4: // Mine bullet
        mBullet->SetBulletType(4);
        mBullet->SetDamage(5);
        mBullet->SetSpeed(700);
        mBullet->SetUpdateTimer(0);
        mBullet->SetCanGoOut(true);
        mBullet->SetIsDestroyable(false);
        break;
    }

    //mBullet->SetTag("Bullet");
    mBullet->SetStartPos(mPos.x + this->GetSize(), mPos.y);
    mBullet->SetPosition(GetStartPos().x, GetStartPos().y);
    mBullet->SetDirection(direction.x, direction.y, mBullet->GetSpeed());

    if (mBullet) {
        scene->AddBullet(mBullet);
    }
}

void Bullet::BulletManager(GameplayScene* scene, Bullet* bullet, int bulletType) {
    if (!bullet) {
        return;
    }

    sf::Vector2f direction = bullet->GetDirection();

    switch (bulletType) {
    case 0: //Classic bullet
        bullet->SetDirection(direction.x, direction.y, bullet->GetSpeed());
        break;

    case 1: //Follow bullet
        if (bullet->ShouldUpdate()) {
            Enemy* closestEnemy = scene->GetClosestEnemy(bullet->GetPosition());
            if (!closestEnemy) {
                return;
            }

            sf::Vector2f targetPosition = closestEnemy->GetPosition();
            sf::Vector2f directionEnemy = targetPosition - bullet->GetPosition();

            float magnitude = std::sqrt(directionEnemy.x * directionEnemy.x + directionEnemy.y * directionEnemy.y);
            if (magnitude != 0) {
                directionEnemy /= magnitude;
            }
            bullet->SetDirection(directionEnemy.x, directionEnemy.y, bullet->GetSpeed());
        }
        break;

    case 2: // Boomerang bullet
        if (bullet->ShouldUpdate()) {
            static bool backtoShip = false;

            if (!backtoShip) {
                bullet->GoToPosition(scene->GetShipPos().x + (scene->GetWindowWidth() / 2), scene->GetShipPos().y, bullet->GetSpeed());
                backtoShip = true;
            }
            else if (bullet->GetPosition().x >= scene->GetShipPos().x + (scene->GetWindowWidth() / 2)) {
                sf::Vector2f targetPosition = scene->GetShipPos();
                sf::Vector2f directionShip = targetPosition - bullet->GetPosition();

                float magnitude = std::sqrt(directionShip.x * directionShip.x + directionShip.y * directionShip.y);
                if (magnitude != 0) {
                    directionShip /= magnitude;
                }
                bullet->SetDirection(directionShip.x, directionShip.y, bullet->GetSpeed());
            }
        }
        break;

    case 3: // Fire projectile
        if (bullet->ShouldUpdate()) {
            sf::Vector2f startPos = bullet->GetStartPos();
            float distance = std::sqrt(std::pow(bullet->GetPosition().x - startPos.x, 2) +
                std::pow(bullet->GetPosition().y - startPos.y, 2));
            if (distance > 400) {
                bullet->Destroy();

				/*auto pBullets = scene->GetBullets();

                auto it = std::find(pBullets.begin(), pBullets.end(), bullet);
                if (it != pBullets.end()) {
                    delete* it;           
                    pBullets.erase(it);   
                }*/
            }
        }
        break;
        

    case 4: // Laser projectile
        bullet->SetDirection(direction.x, direction.y, bullet->GetSpeed());
        break;
    }
}


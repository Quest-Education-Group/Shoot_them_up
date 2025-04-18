#include "pch.h"
#include "Ship.h"
#include "Debug.h"


#include <iostream>



void Ship::OnCollision(Entity* other)
{
    if (other->GetId() == "Enemy") {
        Enemy* enemy = dynamic_cast<Enemy*>(other);
        if (enemy) {
            this->SetHP(this->GetHP() - enemy->GetDamage());
            /*std::cout << "-" << enemy->GetDamage() << "hp->Hp Remaining = " << this->GetHP() << std::endl;*/
        }
    }
    else if (other->GetId() == "EnemyBullet") {
        Bullet* bullet = dynamic_cast<Bullet*>(other);
        if (bullet) {
            this->SetHP(this->GetHP() - bullet->GetDamage());
            /*std::cout << "-" << bullet->GetDamage() << "hp ->Hp Remaining = " << this->GetHP() << std::endl;*/
        }
        /*std::cout << "-25 hp --------------> Hp Remaining = " << this->GetHP() << std::endl;*/
    }
}

void Ship::Move(float deltaTime) {
    const float speed = 200.0f;
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        dirX = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dirX = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        dirY = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dirY = 1;
    }

    float newX = GetPosition().x + dirX * speed * deltaTime;
    float newY = GetPosition().y + dirY * speed * deltaTime;

    const float roadTop = GetScene()->GetWindowHeight() * 0.5f; 
    const float roadBottom = GetScene()->GetWindowHeight();    
    const float roadLeft = 0.0f + (this->GetWidth() / 3);                                
    const float roadRight = GetScene()->GetWindowWidth() - 2 * (this->GetWidth() / 3);

    newX = std::clamp(newX, roadLeft, roadRight);
    newY = std::clamp(newY, roadTop, roadBottom);

    SetPosition(newX, newY);
}

void Ship::SetHP(int hp)
{
    mHP = hp;
}

int Ship::GetHP() const
{
    return mHP;
}

bool Ship::IsShooting() const {
    return mIsFiring;
}

void Ship::SetIsShooting(bool isFiring) {
	mIsFiring = isFiring;
}


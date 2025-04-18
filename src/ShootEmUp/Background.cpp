#include "pch.h"
#include "Background.h"

#include "GameManager.h"
#include "Debug.h"
#include "Utils.h"
#include "AssetManager.h"
#include "Texture.h"
#include "AllScene.h"

Background* Background::Get()
{
	static Background mInstance;

	return &mInstance;
}

void Background::Initialize(int height, int speed, int layer, int nb, std::string path)
{
	mDirection = sf::Vector2f(-1.0f, 0.0f);
	mSpeed = speed;
	mlayer = layer;
	mToDestroy = false;
	mPosY = height;
	Texture* text = AssetManager::Get()->GetTexture(path); 
	SetSprite(text->GetTexture());
	SetScale(GetScene()->GetWindowWidth() + 2, GetScene()->GetWindowHeight() / 2);

	if (nb == 1) 
		SetPosition(0 - GetScene()->GetWindowWidth() / 2, height);
	else
		SetPosition(GetScene()->GetWindowWidth() / 2, height);

}

void Background::InitializeBackground(float width, float height, std::string path)
{
	mDirection = sf::Vector2f(0.0f, 0.0f);
	mToDestroy = false;

	Texture* text = AssetManager::Get()->GetTexture(path);
	SetSprite(text->GetTexture());
	SetScale(width, height);

}

void Background::SetPosition(float x, float y, float ratioX, float ratioY)
{
	x -= GetSize() * ratioX;
	y -= GetSize() * ratioY;

	mImg.mSprite.setPosition(x, y);
}

sf::Vector2f Background::GetPosition(float ratioX, float ratioY) const
{
	sf::Vector2f position = mImg.mSprite.getPosition();

	position.x += GetSize() * ratioX;
	position.y += GetSize() * ratioY;
	
	return position;
}

bool Background::GoToDirection(int x, int y, float speed)
{
	if(speed > 0)
		mSpeed = speed;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);
	sf::Vector2f direction = sf::Vector2f(x - position.x, y - position.y);
	
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	mDirection = direction;

	return true;
}

void Background::Update()
{

	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	sf::Vector2f translation = distance * mDirection;
	mImg.mSprite.move(translation);
	
	if (GetPosition().x + mImg.mWidth <= 0)
		SetPosition(GetScene()->GetWindowWidth()-2, mPosY);

	OnUpdate();
}

Scene* Background::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Background::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}

void Background::SetScale(int width, int height) {
    if (width <= 0 || height <= 0) {
		std::cout << "false" << std::endl;
        return;
    }

	float scaleXDiv = static_cast<float>(width) / mImg.mSprite.getTexture()->getSize().x;
	float scaleYDiv = static_cast<float>(height) / mImg.mSprite.getTexture()->getSize().y;
	mImg.mSprite.setScale(scaleXDiv, scaleYDiv);

	mImg.mWidth = mImg.mSprite.getTexture()->getSize().x * scaleXDiv;
	mImg.mHeight= mImg.mSprite.getTexture()->getSize().y * scaleYDiv;

}

#include "pch.h"
#include "Entity.h"

#include "GameManager.h"
#include "Debug.h"
#include "AssetManager.h"
#include "Texture.h"
#include "AllScene.h"
#include "Utils.h"

Entity* Entity::Get()
{
	static Entity mInstance;

	return &mInstance;
}
void Entity::Initialize(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path)
{
	mDirection = sf::Vector2f(0.0f, 0.0f);
	mSpeed = 0.0f;
	mToDestroy = false;
	mTag = tag;
	mId = id;
	mTarget.isSet = false;
	LoadAnimation(nbAnimation, path);

	mImgAnimation.mSprite.setOrigin(0, 0);
	SetScale(scale);
}

void Entity::LoadAnimation(int nbAnimation, const char* nameAnimation) {
	mImgAnimation.mnbSprites = nbAnimation;
	mImgAnimation.mNameAnimation = nameAnimation;

	if (nbAnimation <= 1) {
		Texture* text = AssetManager::Get()->GetTexture(std::string(nameAnimation) + ".png");
		SetSprite(text->GetTexture());
	}
	else
	{
		for (int i = 0; i < nbAnimation; i++) {

			Texture* text = AssetManager::Get()->GetTexture(nameAnimation + std::to_string(i + 1) + ".png");
			std::pair<std::string, Texture*> animation = { nameAnimation + std::to_string(i + 1) + ".png", text };

			mImgAnimation.mAnimations.push_back(animation);
			if (i <= 0) {
				SetSprite(animation.second->GetTexture());
			}
		}
	}

}

void Entity::InitCercle() {
	sf::Vector2f position = GetPosition(0.5f, 0.5f);
	mCcercle.x = position.x;
	mCcercle.y = position.y;
	mCcercle.radius = mImgAnimation.mSize * 0.6;
}

void Entity::InitAABB() {

	sf::Vector2f position = GetPosition(0.f, 0.f);
	mCAABB.minX = position.x;
	mCAABB.minY = position.y;
	mCAABB.maxX = mImgAnimation.mWidth;
	mCAABB.maxY = mImgAnimation.mHeight;
}

bool Entity::CollidingCercleToCercle(Entity* other) const //cercle -> cercle
{
	sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mCcercle.radius;
	float radius2 = other->mCcercle.radius;

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;
}


bool Entity::CollidingAABBToAABB(Entity* other) const //AABB -> AABB
{
	if ((mCAABB.minX >= other->mCAABB.minX + other->mCAABB.maxX) ||
		(mCAABB.minX + mCAABB.maxX <= other->mCAABB.minX) ||
		(mCAABB.minY >= other->mCAABB.minY + other->mCAABB.maxY) ||
		(mCAABB.minY + mCAABB.maxY <= other->mCAABB.minY))
		return false;
	else
		return true;
}


bool Entity::CollidingCercleToAABB(Entity* other) {
	sf::Vector2f cercleCenter;
	float cercleRadius;
	sf::Vector2f aabbMin;
	sf::Vector2f aabbMax;

	switch (GetTypeCollider())
	{
	case 0:
		cercleCenter = GetPosition(0.5f, 0.5f);
		cercleRadius = mCcercle.radius;
		aabbMin = sf::Vector2f(other->mCAABB.minX, other->mCAABB.minY);
		aabbMax = sf::Vector2f(other->mCAABB.minX + other->mCAABB.maxX, other->mCAABB.minY + other->mCAABB.maxY);
		break;
	case 1:
		cercleCenter = other->GetPosition(0.5f, 0.5f);
		cercleRadius = other->mCcercle.radius;
		aabbMin = sf::Vector2f(mCAABB.minX, mCAABB.minY);
		aabbMax = sf::Vector2f(mCAABB.minX + mCAABB.maxX, mCAABB.minY + mCAABB.maxY);
		break;
	default:
		return false;
	}

	float closestX = std::max(aabbMin.x, std::min(cercleCenter.x, aabbMax.x));
	float closestY = std::max(aabbMin.y, std::min(cercleCenter.y, aabbMax.y));

	float distanceX = cercleCenter.x - closestX;
	float distanceY = cercleCenter.y - closestY;

	return (distanceX * distanceX + distanceY * distanceY) <= (cercleRadius * cercleRadius);
}

bool Entity::SpeCollide(Entity* other) {
	bool condition2 = (GetId() == "Bullet" && other->GetId() == "Player");
	//bool condition3 = GetId() != other->GetId();	
	return condition2;
}


bool Entity::IsColliding(Entity* other) {
	switch (GetSameCollider(other))
	{
	case 0: //cercle -> cercle
		return CollidingCercleToCercle(other);
	case 1: //cercle -> AABB
		return CollidingCercleToAABB(other);
	case 2: //AABB -> AABB
		return CollidingAABBToAABB(other);
	default:
		break;
	}
}

bool Entity::IsInside(float x, float y) const
{
    sf::Vector2f position = GetPosition(0.5f, 0.5f);

	if (GetTypeCollider() == eCercle) {
		float dx = x - position.x;
		float dy = y - position.y;

		return (dx * dx + dy * dy) < (GetSize() * GetSize());
	}

    else if (GetTypeCollider() == eAABB) { 
        return (x >= position.x - (GetWidth() / 2)) &&
               (x <= position.x + (GetWidth() / 2)) &&
               (y >= position.y - (GetHeight() / 2)) &&
               (y <= position.y + (GetHeight() / 2));
    }

    return false; 
}


void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
	x -= GetSize() * ratioX;
	y -= GetSize() * ratioY;

	mImgAnimation.mSprite.setPosition(x, y);
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	sf::Vector2f position = mImgAnimation.mSprite.getPosition();

	position.x += GetSize() * ratioX;
	position.y += GetSize() * ratioY;
	return position;
}

bool Entity::GoToDirection(int x, int y, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);
	sf::Vector2f direction = sf::Vector2f(x - position.x, y - position.y);

	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	mDirection = direction;

	return true;
}

bool Entity::GoToPosition(int x, int y, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	mTarget.position = { x, y };
	mTarget.distance = Utils::GetDistance(position.x, position.y, x, y);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = sf::Vector2f(x, y);
}


void Entity::Update(float dt)
{
	//DrawCollider(*GameManager::Get()->GetWindow());
	OnUpdate();
}

void Entity::FixeUpdate(float dt) {

	if (GetTypeCollider() == Entity::eCercle)
		InitCercle();
	else if (GetTypeCollider() == Entity::eAABB)
		InitAABB();


	float distance = dt * mSpeed;
	sf::Vector2f translation = distance * mDirection;
	GetSprite().move(translation);

	if (mTarget.isSet)
	{
		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y, 0.5f, 0.5f);
			mDirection = sf::Vector2f(0.f, 0.f);
			mTarget.isSet = false;
		}
	}
	SpriteAnimator();

}

Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}


void Entity::SpriteAnimator() {
	mImgAnimation.mDuration -= GetDeltaTime();

	if (mImgAnimation.mDuration <= 0) {
		mImgAnimation.mDuration += 0.1f;

		mImgAnimation.mIndex = (mImgAnimation.mIndex + 1) % mImgAnimation.mnbSprites;

		std::string newFameAnimation = mImgAnimation.mNameAnimation + std::to_string(mImgAnimation.mIndex) + ".png";

		for (const auto& animation : mImgAnimation.mAnimations) {

			if (animation.first == newFameAnimation) {
				sf::Vector2f currentPosition = GetSprite().getPosition();

				SetSprite(animation.second->GetTexture());
				SetScale(mImgAnimation.mScale);
				GetSprite().setPosition(currentPosition);
			}
		}
	}
}

int Entity::GetSameCollider(Entity* other) {
	return GetTypeCollider() + other->GetTypeCollider();
}

void Entity::SetScale(float scaleFactor) {
	mImgAnimation.mScale = scaleFactor;
	GetSprite().setScale(scaleFactor, scaleFactor);

	mImgAnimation.mWidth = GetSprite().getTexture()->getSize().x * scaleFactor;
	mImgAnimation.mHeight = GetSprite().getTexture()->getSize().y * scaleFactor;

	mImgAnimation.mSize = (mImgAnimation.mWidth + mImgAnimation.mHeight) / 2;
}

//void Entity::DrawCollider(sf::RenderWindow& window) const
//{
//	sf::RectangleShape rect;
//	rect.setSize(sf::Vector2f(GetWidth(), GetHeight()));
//	rect.setPosition(GetPosition(0.5f, 0.5f) - sf::Vector2f(GetWidth() / 2, GetHeight() / 2));
//	rect.setFillColor(sf::Color::Red); // Transparent
//	rect.setOutlineColor(sf::Color::Red);     // Contour rouge
//	rect.setOutlineThickness(1.0f);
//
//	window.draw(rect);
//}

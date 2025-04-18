#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <list>
class Texture;
namespace sf
{
    class Shape;
    class Color;
}

struct ImageEntityAnimation
{
    std::list<std::pair<std::string, Texture*>> mAnimations;
    sf::Sprite mSprite;
    int mIndex;
    int mnbSprites;
    float mWidth;
    float mHeight;
    float mDuration = 0.1f;
    float mSize;
    float mScale = 1.0f;
    const char* mNameAnimation;
};

struct CercleCollider
{
    float x;
    float y;
    float radius;
};

struct AABBCollider
{
    float minX;
    float minY;
    float maxX;
    float maxY;
};

class GameManager;
class Scene;
class AssetManager;

class Entity
{
    struct Target
    {
        sf::Vector2i position;
        float distance;
        bool isSet;
    };

protected:
    sf::Vector2f mDirection;
    Target mTarget;
    ImageEntityAnimation mImgAnimation;

    CercleCollider mCcercle;
    AABBCollider mCAABB;
    std::string mPath;
    float mSpeed;
    bool mToDestroy;
    sf::String mTag;
    sf::String mId;


public:
    static Entity* Get();
    enum TypeEntityCollider
    {
        eCercle,
        eAABB
    };

    TypeEntityCollider eTypeEntityCollider;
    bool GoToDirection(int x, int y, float speed = -1.f);
    bool GoToPosition(int x, int y, float speed = -1.f);

    void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
    void SetDirection(float x, float y, float speed = -1.f);

    sf::Vector2f GetDirection() { return mDirection; }
    int GetSpeed() { return mSpeed; }

    void SetScale(float scaleFactor);

    void SetSpeed(float speed) { mSpeed = speed; }
    void SetTag(sf::String tag) { mTag = tag; }
    sf::String GetTag() { return mTag; }

    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;

    sf::Sprite& GetSprite() { return mImgAnimation.mSprite; }
    float GetSize() const { return mImgAnimation.mSize; }
    float GetWidth() const { return mImgAnimation.mWidth; }
    float GetHeight() const { return mImgAnimation.mHeight; }

    void SetSprite(sf::Sprite spt) { mImgAnimation.mSprite = spt; };


    void LoadAnimation(int nbAnimation, const char* nameAnimation);
    void SpriteAnimator();

    bool IsTag(sf::String tag) const { return mTag == tag; }
    bool IsId(sf::String id) const { return mId == id; }
    sf::String GetId() const { return mId; }
    bool IsColliding(Entity* other);

    void InitCercle();
    void InitAABB();

    bool CollidingCercleToCercle(Entity* other) const;
    bool CollidingAABBToAABB(Entity* other) const;
    bool CollidingCercleToAABB(Entity* other);

    bool SpeCollide(Entity* other);
    bool CanCollide(Entity* other) { return SpeCollide(other); }

    bool IsInside(float x, float y) const;

    void Destroy() { mToDestroy = true; }
    bool ToDestroy() const { return mToDestroy; }


    void SetTypeEntityCollider(TypeEntityCollider type) { eTypeEntityCollider = type; }
    TypeEntityCollider GetTypeCollider() const { return eTypeEntityCollider; }
    int GetSameCollider(Entity* other);

	//void DrawCollider(sf::RenderWindow& window) const;


    template<typename T>
    T* GetScene() const;

    Scene* GetScene() const;
    float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path);


protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnFixeUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
    virtual void OnInitialize() {};

private:
    void Update(float dt);
    void FixeUpdate(float dt);
    void Initialize(int nbAnimation, float scale, sf::String tag, sf::String id, const char* path);

    friend GameManager;
    friend AssetManager;
    friend Scene;
};

#include "Entity.inl"
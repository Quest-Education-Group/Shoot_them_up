#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace sf 
{
	class Shape;
    class Color;
}

struct ImageBackground
{
    sf::Sprite mSprite;
    float mWidth;
    float mHeight;
    float mSize;
};

class GameManager;
class Scene;
class AssetManager;

class Background
{
protected:
    sf::Vector2f mDirection;
    float mSpeed;
    float mPosY;
    bool mToDestroy;
    int mlayer;
    ImageBackground mImg;

public:
    static Background* Get();
	bool GoToDirection(int x, int y, float speed = -1.f);
    void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);

    void SetScale(int width, int height);
	void SetSpeed(float speed) { mSpeed = speed; }
    float GetSize() const { return mImg.mSize; }
    float GetLayer() const { return mlayer; }
    sf::Sprite& GetSprite() { return mImg.mSprite; }

    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;

    void Destroy() { mToDestroy = true; }
    bool ToDestroy() const { return mToDestroy; }
    void SetSprite(sf::Sprite spt) { mImg.mSprite = spt; };

	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateParalax(int height, int speed, int layer, std::string path);

    template<typename T>
    T* CreateBackground(float width, float height, std::string path);


protected:
    Background() = default;
    ~Background() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Background* collidedWith) {};
	virtual void OnInitialize() {};
	
private:
    void Update();
    void Initialize(int height, int speed, int layer, int nb, std::string path);
    void InitializeBackground(float width, float height, std::string path);

    friend GameManager;
    friend AssetManager;
    friend Scene;
};

#include "Background.inl"
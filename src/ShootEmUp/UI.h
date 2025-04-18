#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace sf 
{
	class Shape;
    class Color;
}

struct ImageUI
{
    sf::Sprite mSprite;
    float mWidth;
    float mHeight;
    float mSize;
};

class GameManager;
class Scene;
class AssetManager;

class UI
{
protected:
    bool mToDestroy;
    ImageUI mImageUI;
    sf::String mTag;

public:
    static UI* Get();
    void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;

    void ActionUi(UI* ui);
    
    void SetScale(int width, int height);
    float GetSize() const { return mImageUI.mSize; }
    sf::Sprite& GetSprite() { return mImageUI.mSprite; }
    void SetTag(sf::String tag) { mTag = tag; }
    sf::String GetTag() { return mTag; }


    void Destroy() { mToDestroy = true; }
    bool ToDestroy() const { return mToDestroy; }
    void SetSprite(sf::Sprite spt) { mImageUI.mSprite = spt; };

	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;

    template<typename T>
    T* CreateUI(int scaleX, int scaleY, sf::String tag, std::string path);


protected:
    UI() = default;
    ~UI() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(UI* collidedWith) {};
	virtual void OnInitialize() {};
	
private:
    void Update();
    void Initialize(int scaleX, int scaleY, sf::String tag, std::string path);

    friend GameManager;
    friend AssetManager;
    friend Scene;
};

#include "UI.inl"
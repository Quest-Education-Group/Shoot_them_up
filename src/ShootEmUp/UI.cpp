#include "pch.h"
#include "UI.h"

#include "GameManager.h"
#include "Debug.h"
#include "Utils.h"
#include "AssetManager.h"
#include "Texture.h"
#include "AllScene.h"

UI* UI::Get()
{
	static UI mInstance;

	return &mInstance;
}

void UI::Initialize(int scaleX, int scaleY, sf::String tag, std::string path)
{
	mToDestroy = false;
	mTag = tag;
	Texture* text = AssetManager::Get()->GetTexture(path);
	SetSprite(text->GetTexture());
	SetScale(scaleX, scaleY);

}

void UI::SetPosition(float x, float y, float ratioX, float ratioY)
{
	x -= GetSize() * ratioX;
	y -= GetSize() * ratioY;

	mImageUI.mSprite.setPosition(x, y);
}

sf::Vector2f UI::GetPosition(float ratioX, float ratioY) const
{
	sf::Vector2f position = mImageUI.mSprite.getPosition();

	position.x += GetSize() * ratioX;
	position.y += GetSize() * ratioY;
	
	return position;
}

void UI::ActionUi(UI* ui) {
	if (ui->GetTag() == "pause") {
		GameManager::Get()->SetInPause(!GameManager::Get()->GetInPause());
	}
}

void UI::Update()
{
	Debug::DrawRectangle(GetPosition(0, 0).x, GetPosition(0, 0).y, mImageUI.mWidth, mImageUI.mHeight, sf::Color::Red);
	OnUpdate();
}

Scene* UI::GetScene() const
{
	return GameManager::Get()->GetScene();
}

void UI::SetScale(int width, int height) {
    if (width <= 0 || height <= 0) {
		std::cout << "false" << std::endl;
        return;
    }

	float scaleXDiv = static_cast<float>(width) / mImageUI.mSprite.getTexture()->getSize().x;
	float scaleYDiv = static_cast<float>(height) / mImageUI.mSprite.getTexture()->getSize().y;

	mImageUI.mSprite.setScale(scaleXDiv, scaleYDiv);

	mImageUI.mWidth = mImageUI.mSprite.getTexture()->getSize().x * scaleXDiv;
	mImageUI.mHeight= mImageUI.mSprite.getTexture()->getSize().y * scaleYDiv;

	mImageUI.mSize = (mImageUI.mWidth + mImageUI.mHeight)/2;

}

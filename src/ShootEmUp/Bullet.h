#include "Entity.h"
#include "Enemy.h"
#include "GameplayScene.h"

#define PI 3.14159265

class Bullet : public Entity
{
public:
	void OnCollision(Entity* other) override;

	void SetDamage(int hp);
	int GetDamage() const;

	void SetBulletType(int bulletType);
	int GetBulletType() const;

	bool OutOfScreen();

	void SetUpdateTimer(int threshold);
	bool ShouldUpdate();

	void SetCanGoOut(bool canGoOut);
	bool GetCanGoOut() const;

	void SetStartPos(float x, float y);
	sf::Vector2f GetStartPos() const;

	void SetIsDestroyable(bool destroyable);
	bool GetIsDestroyable();

	virtual void BulletCreator(GameplayScene* scene, Bullet* mBullet, int BulletType, sf::Vector2f direction, sf::Vector2f mPos);
	virtual void BulletManager(GameplayScene* scene, Bullet* Bullet, int BulletType);

	void Shoot(GameplayScene* scene, sf::Vector2f mPos, sf::String mTeam, int bulletType, int bulletNb);

private:
	int mDamage;
	int mBulletType;
	int mIsDestroyable;
	float mLifetime;
	sf::Vector2f mStartPosition;

	bool mCanGoOut;

	int mUpdateCounter = 0; 
	int mUpdateTimer;
};

#pragma once
#include "Entity.h"
#include "Background.h"
#include "UI.h"


class MoteurEntity : public Entity
{
public:
	void OnCollision(Entity* other) override;
};

class MoteurBackground : public Background
{
public:
};

class MoteurUI : public UI
{
public:
};


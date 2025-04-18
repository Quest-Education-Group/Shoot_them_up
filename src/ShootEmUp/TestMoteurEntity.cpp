#include "pch.h"
#include "TestMoteurEntity.h"
#include "debug.h"

#include <iostream>

void MoteurEntity::OnCollision(Entity* other)
{
	std::cout << "OnCollision: " << GetTypeCollider() << std::endl;
}
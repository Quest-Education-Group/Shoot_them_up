#pragma once
#include "MenuManager.h"

class LoseScene : public MenuManager
{
public:
    static LoseScene* Get();
    void OnInitialize() override;
    void OnUpdate() override;

    void DrawScore();
};



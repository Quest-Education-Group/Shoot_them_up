#pragma once
#include "MenuManager.h"

class StartScene : public MenuManager
{
public:
    static StartScene* Get();
    void OnInitialize() override;
};

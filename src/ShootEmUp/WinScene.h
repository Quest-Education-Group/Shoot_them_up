#pragma once
#include "MenuManager.h"

class WinScene : public MenuManager
{
public:
    static WinScene* Get();
    void OnInitialize() override;
	void OnUpdate() override;

	void DrawScore();
};

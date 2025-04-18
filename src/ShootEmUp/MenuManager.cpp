#include "pch.h"
#include "MenuManager.h"

#include "Entity.h"
#include "Background.h"

void MenuManager::InitializeBackground(const std::string& bgPath1, const std::string& bgPath2) {
    pBackground1 = CreateBackground<Background>(GetWindowWidth(), GetWindowHeight(), bgPath1);

    if (!bgPath2.empty()) {
        pBackground2 = CreateBackground<Background>(250, 100, bgPath2);
        pBackground2->SetPosition((GetWindowWidth() / 2) - (250 / 2), 100);
    }
}

void MenuManager::InitializeButton(int index, const char* buttonPath, float xOffset, float yOffset) {
    Entity* button = CreateEntity<Entity>(1, 0.5f, "Entity", "Button", buttonPath);
    button->SetTypeEntityCollider(Entity::eAABB);
    button->SetPosition((GetWindowWidth() / 2) - (button->GetWidth() / 2) + xOffset, yOffset);
    buttons.push_back(button);
}

void MenuManager::OnEvent(const sf::Event& event) {
    if (event.type != sf::Event::EventType::MouseButtonPressed)
        return;

    if (event.mouseButton.button == sf::Mouse::Button::Left) {
        for (size_t i = 0; i < buttons.size(); ++i) {
            sf::Vector2f patchedPos(event.mouseButton.x - (buttons[i]->GetWidth() / 4),
                event.mouseButton.y + buttons[i]->GetHeight());
            if (buttons[i]->IsInside(patchedPos.x, patchedPos.y)) {
                /*std::cout << "Button " << i << " clicked" << std::endl;*/
                buttonActions[i]();
                return;
            }
        }
    }
}

void MenuManager::SetFinalScore(int score) {
    finalScore = score;
}

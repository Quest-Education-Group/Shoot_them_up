#include "pch.h"
#include "LevelManager.h"
#include <fstream>
#include "Enemy.h"
#include "Debug.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>



void LevelManager::OnInitialize()
{
	Read();
}



void LevelManager::OnEvent(const sf::Event& event)
{

}

void LevelManager::OnUpdate()
{

}

void LevelManager::Read() {
    std::ifstream monFlux(lien);

    if (monFlux)
    {
        std::cout << "fichier ouvert" << std::endl;
        std::string ligne1, ligne2, lignetop;
        bool attente = false;

        while (std::getline(monFlux, ligne1) && std::getline(monFlux, ligne2) && std::getline(monFlux, lignetop))
        {
            size_t taille = std::max({ ligne1.length(), ligne2.length(), lignetop.length() });
            for (size_t i = 0; i < taille; ++i)
            {
                if (i < ligne1.length() && ligne1[i] != ' ')
                {
                    std::cout << ligne1[i];
                    if (ligne1[i] == '1')
                    {
                        std::cout << " un ennemi pop" << std::endl;
                    }
                    else if (ligne1[i] == '-')
                    {
                        std::cout << " pas d'ennemi" << std::endl;
                    }
                }
                if (i < ligne2.length() && ligne2[i] != ' ')
                {
                    std::cout << ligne2[i];
                    if (ligne2[i] == '1')
                    {
                        std::cout << " un ennemi pop" << std::endl;
                    }
                    else if (ligne2[i] == '-')
                    {
                        std::cout << " pas d'ennemi" << std::endl;
                    }
                }
                if (i < lignetop.length() && lignetop[i] != ' ')
                {
                    std::cout << lignetop[i];
                    if (lignetop[i] == '1')
                    {
                        std::cout << " un ennemi pop" << std::endl;
                    }
                    else if (lignetop[i] == '-')
                    {
                        std::cout << " pas d'ennemi" << std::endl;
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            if (!attente)
            {
                std::cout << "Attente de quelques secondes..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(15));
                attente = true;
            }
            else
            {
                attente == false;
            }
        }

    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }
}



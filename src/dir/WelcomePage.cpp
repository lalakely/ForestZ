#include "../include/WelcomePage.hpp"
#include <iostream>

WelcomePage::WelcomePage(Ogre::RenderWindow* window)
    : window(window),
      trayManager(nullptr),
      playButton(nullptr),
      quitButton(nullptr),
      settingsButton(nullptr),
      multiplayerButton(nullptr),
      titleLabel(nullptr)
{
    std::cout << "WelcomePage constructor called" << std::endl;
}

WelcomePage::~WelcomePage()
{
    std::cout << "WelcomePage destructor called" << std::endl;
    if (trayManager) {
        delete trayManager;
        trayManager = nullptr;
    }
}

void WelcomePage::initialize()
{
    std::cout << "WelcomePage::initialize() - Starting initialization" << std::endl;
    
    try {
        // Créer le TrayManager avec un nom unique
        trayManager = new OgreBites::TrayManager("WelcomeTray", window);
        if (!trayManager) {
            throw std::runtime_error("Failed to create TrayManager");
        }
        std::cout << "TrayManager created successfully" << std::endl;

        // Configurer le TrayManager
        trayManager->setListener(this);
        trayManager->showCursor();
        std::cout << "TrayManager configured" << std::endl;

        // Créer les boutons
        createButtons();
        std::cout << "Buttons created" << std::endl;

        // Forcer l'affichage de tous les éléments
        trayManager->showAll();
        std::cout << "All elements shown" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in initialize: " << e.what() << std::endl;
    }
}

void WelcomePage::createButtons()
{
    std::cout << "WelcomePage::createButtons() - Starting button creation" << std::endl;
    
    try {
        // Créer le titre
        titleLabel = trayManager->createLabel(OgreBites::TL_CENTER, "TitleLabel", "FOREST Z", 250);
        if (!titleLabel) {
            throw std::runtime_error("Failed to create title label");
        }
        std::cout << "Title label created" << std::endl;

        // Créer les boutons
        playButton = trayManager->createButton(OgreBites::TL_CENTER, "PlayButton", "Jouer", 200);
        if (!playButton) {
            throw std::runtime_error("Failed to create play button");
        }
        std::cout << "Play button created" << std::endl;

        multiplayerButton = trayManager->createButton(OgreBites::TL_CENTER, "MultiplayerButton", "Multijoueur", 200);
        if (!multiplayerButton) {
            throw std::runtime_error("Failed to create multiplayer button");
        }
        std::cout << "Multiplayer button created" << std::endl;

        settingsButton = trayManager->createButton(OgreBites::TL_CENTER, "SettingsButton", "Paramètres", 200);
        if (!settingsButton) {
            throw std::runtime_error("Failed to create settings button");
        }
        std::cout << "Settings button created" << std::endl;

        quitButton = trayManager->createButton(OgreBites::TL_CENTER, "QuitButton", "Quitter", 200);
        if (!quitButton) {
            throw std::runtime_error("Failed to create quit button");
        }
        std::cout << "Quit button created" << std::endl;

        // Positionner les boutons verticalement
        float buttonSpacing = 20.0f;
        float currentY = -100.0f; // Commencer en haut

        // Les boutons sont automatiquement positionnés par le TrayManager
        // Pas besoin de les positionner manuellement

        // Forcer l'affichage de tous les boutons
        playButton->show();
        multiplayerButton->show();
        settingsButton->show();
        quitButton->show();
        titleLabel->show();
        std::cout << "All buttons shown" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in createButtons: " << e.what() << std::endl;
    }
}

void WelcomePage::show()
{
    std::cout << "WelcomePage::show() - Showing welcome page" << std::endl;
    try {
        if (!trayManager) {
            throw std::runtime_error("TrayManager is null in show()");
        }
        trayManager->showAll();
        trayManager->showCursor();
        std::cout << "All elements shown" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in show: " << e.what() << std::endl;
    }
}

void WelcomePage::hide()
{
    std::cout << "WelcomePage::hide() - Hiding welcome page" << std::endl;
    try {
        if (!trayManager) {
            throw std::runtime_error("TrayManager is null in hide()");
        }
        trayManager->hideAll();
        trayManager->hideCursor();
        std::cout << "All elements hidden" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in hide: " << e.what() << std::endl;
    }
}

void WelcomePage::buttonHit(OgreBites::Button* button)
{
    std::cout << "Button hit: " << (button ? button->getName() : "unknown") << std::endl;
    
    if (button == playButton && playCallback) {
        std::cout << "Starting game..." << std::endl;
        playCallback();
    }
    else if (button == quitButton && quitCallback) {
        std::cout << "Quitting game..." << std::endl;
        quitCallback();
    }
    else if (button == settingsButton && settingsCallback) {
        std::cout << "Opening settings..." << std::endl;
        settingsCallback();
    }
    else if (button == multiplayerButton && multiplayerCallback) {
        std::cout << "Opening multiplayer menu..." << std::endl;
        multiplayerCallback();
    }
} 
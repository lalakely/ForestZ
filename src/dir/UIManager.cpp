#include "../include/UIManager.hpp"
#include <iostream>

UIManager::UIManager(Ogre::SceneManager* scnMgr, Ogre::RenderWindow* window)
    : scnMgr(scnMgr),
      window(window),
      welcomePage(nullptr),
      hud(nullptr),
      minimap(nullptr),
      crosshair(nullptr)
{
}

UIManager::~UIManager()
{
    delete crosshair;
    delete hud;
    delete minimap;
    delete welcomePage;
}

void UIManager::initialize()
{
    initializeWelcomePage();
    // HUD, minimap and crosshair will be initialized when game starts
}

void UIManager::initializeWelcomePage()
{
    try {
        if (!window) {
            throw std::runtime_error("Failed to get render window");
        }

        // Create and initialize the welcome page
        welcomePage = new WelcomePage(window);
        if (!welcomePage) {
            throw std::runtime_error("Failed to create WelcomePage");
        }
        
        welcomePage->initialize();
        
        // Show the welcome page
        welcomePage->show();
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing welcome page: " << e.what() << std::endl;
        Ogre::LogManager::getSingleton().logMessage(
            "Error initializing welcome page: " + std::string(e.what()));
    }
}

void UIManager::initializeHUD()
{
    hud = new HUD();
    if (hud) {
        hud->initialize();
    }
}

void UIManager::initializeMinimap()
{
    minimap = new Minimap();
    if (minimap) {
        minimap->initialize(scnMgr);
        // Adjust position based on window size
        if (window) {
            minimap->adjustPosition(window->getWidth());
        }
    }
}

void UIManager::initializeCrosshair()
{
    crosshair = new Crosshair();
    if (crosshair) {
        crosshair->initialize(scnMgr);
    }
}

void UIManager::update(float deltaTime)
{
    // Update UI elements
    if (crosshair && window) {
        crosshair->update(window->getWidth(), window->getHeight());
    }
}

void UIManager::updateMinimap(const Ogre::Vector3& playerPos, 
                             const std::vector<Ogre::Vector3>& zombiePositions,
                             const std::vector<Ogre::Vector3>& treePositions)
{
    if (minimap) {
        minimap->update(playerPos, zombiePositions, treePositions);
    }
}

void UIManager::updateHealthBar(float currentHealth, float maxHealth)
{
    if (hud) {
        hud->updateHealthBar(currentHealth, maxHealth);
    }
}

void UIManager::updateEnergyBar(float currentEnergy, float maxEnergy)
{
    if (hud) {
        hud->updateEnergyBar(currentEnergy, maxEnergy);
    }
}

void UIManager::showWelcomePage()
{
    if (welcomePage) {
        welcomePage->show();
    }
}

void UIManager::hideWelcomePage()
{
    if (welcomePage) {
        welcomePage->hide();
    }
}

void UIManager::setPlayCallback(std::function<void()> callback)
{
    if (welcomePage) {
        welcomePage->setPlayCallback(callback);
    }
}

void UIManager::setMultiplayerCallback(std::function<void()> callback)
{
    if (welcomePage) {
        welcomePage->setMultiplayerCallback(callback);
    }
}

void UIManager::setSettingsCallback(std::function<void()> callback)
{
    if (welcomePage) {
        welcomePage->setSettingsCallback(callback);
    }
}

void UIManager::setQuitCallback(std::function<void()> callback)
{
    if (welcomePage) {
        welcomePage->setQuitCallback(callback);
    }
}

OgreBites::TrayManager* UIManager::getTrayManager() const
{
    if (welcomePage) {
        return welcomePage->getTrayManager();
    }
    return nullptr;
}

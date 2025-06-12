#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include "../include/WelcomePage.hpp"
#include "../include/HUD.hpp"
#include "../include/Minimap.hpp"
#include "../include/Crosshair.hpp"
#include <functional>

class UIManager {
public:
    UIManager(Ogre::SceneManager* scnMgr, Ogre::RenderWindow* window);
    ~UIManager();

    void initialize();
    void initializeWelcomePage();
    void initializeHUD();
    void initializeMinimap();
    void initializeCrosshair();
    
    void update(float deltaTime);
    void updateMinimap(const Ogre::Vector3& playerPos, 
                      const std::vector<Ogre::Vector3>& zombiePositions,
                      const std::vector<Ogre::Vector3>& treePositions);
    void updateHealthBar(float currentHealth, float maxHealth);
    void updateEnergyBar(float currentEnergy, float maxEnergy);
    
    void showWelcomePage();
    void hideWelcomePage();
    
    // Callbacks
    void setPlayCallback(std::function<void()> callback);
    void setMultiplayerCallback(std::function<void()> callback);
    void setSettingsCallback(std::function<void()> callback);
    void setQuitCallback(std::function<void()> callback);
    
    // Getters
    WelcomePage* getWelcomePage() const { return welcomePage; }
    HUD* getHUD() const { return hud; }
    Minimap* getMinimap() const { return minimap; }
    Crosshair* getCrosshair() const { return crosshair; }
    OgreBites::TrayManager* getTrayManager() const;

private:
    Ogre::SceneManager* scnMgr;
    Ogre::RenderWindow* window;
    
    WelcomePage* welcomePage;
    HUD* hud;
    Minimap* minimap;
    Crosshair* crosshair;
};

#endif // UI_MANAGER_HPP

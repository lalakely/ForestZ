#pragma once

#include <Ogre.h>
#include <OgreBitesPrerequisites.h>
#include <OgreTrays.h>
#include <functional>

class WelcomePage : public OgreBites::TrayListener {
public:
    WelcomePage(Ogre::RenderWindow* window);
    ~WelcomePage();

    void initialize();
    void show();
    void hide();

    // Callbacks
    void setPlayCallback(std::function<void()> callback) { playCallback = callback; }
    void setMultiplayerCallback(std::function<void()> callback) { multiplayerCallback = callback; }
    void setSettingsCallback(std::function<void()> callback) { settingsCallback = callback; }
    void setQuitCallback(std::function<void()> callback) { quitCallback = callback; }

    // Getter pour le TrayManager
    OgreBites::TrayManager* getTrayManager() const { return trayManager; }

    // Override de la méthode buttonHit de TrayListener
    void buttonHit(OgreBites::Button* button) override;

private:
    void createButtons();

    Ogre::RenderWindow* window;
    OgreBites::TrayManager* trayManager;
    OgreBites::Button* playButton;
    OgreBites::Button* quitButton;
    OgreBites::Button* settingsButton;
    OgreBites::Button* multiplayerButton;
    OgreBites::Label* titleLabel;

    // Callbacks
    std::function<void()> playCallback;
    std::function<void()> quitCallback;
    std::function<void()> settingsCallback;
    std::function<void()> multiplayerCallback;
}; 
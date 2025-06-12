#ifndef HUD_HPP
#define HUD_HPP

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

class HUD {
public:
    HUD();
    ~HUD();

    void initialize();
    void show();
    void hide();
    void updateEnergyBar(float currentEnergy, float maxEnergy);
    void updateHealthBar(float currentHealth, float maxHealth);

private:
    Ogre::Overlay* hudOverlay;
    Ogre::OverlayContainer* bar1;
    Ogre::OverlayContainer* bar2;
    Ogre::OverlayContainer* energyFill;
    Ogre::OverlayContainer* healthFill;
    
    void createBars();
};

#endif 
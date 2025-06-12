#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <vector>

class Minimap {
public:
    Minimap();
    ~Minimap();

    void initialize(Ogre::SceneManager* scnMgr);
    void update(const Ogre::Vector3& playerPos, 
                const std::vector<Ogre::Vector3>& zombiePositions,
                const std::vector<Ogre::Vector3>& treePositions);
    void show() { if(minimapOverlay) minimapOverlay->show(); }
    void hide() { if(minimapOverlay) minimapOverlay->hide(); }
    void adjustPosition(unsigned int screenWidth);

private:
    void createMinimapIcon(const std::string& name, const Ogre::ColourValue& color);
    void updateIconPosition(const std::string& name, const Ogre::Vector3& worldPos);

    Ogre::Overlay* minimapOverlay;
    Ogre::OverlayContainer* minimapBackground;
    Ogre::OverlayContainer* playerIcon;
    std::vector<Ogre::OverlayContainer*> zombieIcons;
    std::vector<Ogre::OverlayContainer*> treeIcons;
    Ogre::Vector3 playerLastPosition;

    // Paramètres du minimap
    const float MINIMAP_SIZE = 200.0f;  // Taille en pixels
    const float MINIMAP_SCALE = 0.1f;   // Échelle monde->minimap
    const float MINIMAP_MARGIN = 20.0f;  // Marge depuis le coin de l'écran
};

#endif 
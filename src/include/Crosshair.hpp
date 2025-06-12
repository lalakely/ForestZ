#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP

#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgrePanelOverlayElement.h>

class Crosshair {
public:
    Crosshair();
    ~Crosshair();

    void initialize(Ogre::SceneManager* scnMgr);
    void update(int screenWidth, int screenHeight);

private:
    void createCrosshairElements();

    Ogre::Overlay* mOverlay;
    Ogre::OverlayContainer* mContainer;
    Ogre::PanelOverlayElement* mCrosshairVertical;
    Ogre::PanelOverlayElement* mCrosshairHorizontal;
    float mCenterX;
    float mCenterY;
};

#endif // CROSSHAIR_HPP 
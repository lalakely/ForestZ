#include "Crosshair.hpp"
#include <OgreOverlayManager.h>
#include <OgreRoot.h>

Crosshair::Crosshair() 
    : mOverlay(nullptr)
    , mContainer(nullptr)
    , mCrosshairVertical(nullptr)
    , mCrosshairHorizontal(nullptr)
    , mCenterX(0.5f)
    , mCenterY(0.5f)
{
}

Crosshair::~Crosshair() {
    if (mOverlay) {
        Ogre::OverlayManager::getSingleton().destroy(mOverlay);
    }
    if (mContainer) {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(mContainer);
    }
    if (mCrosshairVertical) {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(mCrosshairVertical);
    }
    if (mCrosshairHorizontal) {
        Ogre::OverlayManager::getSingleton().destroyOverlayElement(mCrosshairHorizontal);
    }
}

void Crosshair::initialize(Ogre::SceneManager* scnMgr) {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    // Créer l'overlay
    mOverlay = overlayManager.create("CrosshairOverlay");
    
    // Créer le conteneur
    mContainer = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "CrosshairContainer"));
    mContainer->setMetricsMode(Ogre::GMM_RELATIVE);
    mContainer->setPosition(0, 0);
    mContainer->setDimensions(1, 1);
    
    // Créer les éléments du viseur
    createCrosshairElements();
    
    // Assembler l'overlay
    mContainer->addChild(mCrosshairVertical);
    mContainer->addChild(mCrosshairHorizontal);
    mOverlay->add2D(mContainer);
    
    // Afficher l'overlay
    mOverlay->setZOrder(500);
    mOverlay->show();
}

void Crosshair::createCrosshairElements() {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    // Créer la ligne verticale
    mCrosshairVertical = static_cast<Ogre::PanelOverlayElement*>(
        overlayManager.createOverlayElement("Panel", "CrosshairVertical"));
    mCrosshairVertical->setMetricsMode(Ogre::GMM_RELATIVE);
    mCrosshairVertical->setPosition(0.4795f, 0.53f); // Centré horizontalement
    mCrosshairVertical->setDimensions(0.001f, 0.04f); // Ligne fine et courte
    mCrosshairVertical->setMaterialName("BaseWhite"); // Couleur blanche par défaut
    
    // Créer la ligne horizontale
    mCrosshairHorizontal = static_cast<Ogre::PanelOverlayElement*>(
        overlayManager.createOverlayElement("Panel", "CrosshairHorizontal"));
    mCrosshairHorizontal->setMetricsMode(Ogre::GMM_RELATIVE);
    mCrosshairHorizontal->setPosition(0.46f, 0.5495f); // Centré verticalement
    mCrosshairHorizontal->setDimensions(0.04f, 0.001f); // Ligne fine et courte
    mCrosshairHorizontal->setMaterialName("BaseWhite"); // Couleur blanche par défaut
}

void Crosshair::update(int screenWidth, int screenHeight) {
    // Mettre à jour la position du centre en coordonnées d'écran
    mCenterX = screenWidth * 0.5f;
    mCenterY = screenHeight * 0.5f;
} 
#include "../include/HUD.hpp"
#include <sstream>

HUD::HUD()
    : hudOverlay(nullptr)
    , bar1(nullptr)
    , bar2(nullptr)
    , energyFill(nullptr)
    , healthFill(nullptr)
{
}

HUD::~HUD() {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    if (healthFill) overlayManager.destroyOverlayElement(healthFill);
    if (energyFill) overlayManager.destroyOverlayElement(energyFill);
    if (bar1) overlayManager.destroyOverlayElement(bar1);
    if (bar2) overlayManager.destroyOverlayElement(bar2);
    if (hudOverlay) overlayManager.destroy(hudOverlay);
}

void HUD::initialize() {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    // Créer l'overlay principal
    hudOverlay = overlayManager.create("PlayerHUD");
    
    createBars();
    
    // Z-order plus bas pour que les barres soient sous les textes
    hudOverlay->setZOrder(100);
    show();
}

void HUD::createBars() {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    // Créer la première barre (fond de vie)
    bar1 = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "Bar1"));
    bar1->setMetricsMode(Ogre::GMM_PIXELS);
    bar1->setPosition(10, 980);
    bar1->setDimensions(500, 50);
    bar1->setMaterialName("Core/StatsBlockCenter");
    bar1->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.7f)); // Fond noir semi-transparent
    
    // Créer la barre de remplissage de vie
    healthFill = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "HealthFill"));
    healthFill->setMetricsMode(Ogre::GMM_PIXELS);
    healthFill->setPosition(0, 0); // Position relative au parent (bar1)
    healthFill->setDimensions(500, 50);
    healthFill->setMaterialName("Core/HealthBar");
    
    // Créer la deuxième barre (fond d'énergie)
    bar2 = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "Bar2"));
    bar2->setMetricsMode(Ogre::GMM_PIXELS);
    bar2->setPosition(10, 1040);
    bar2->setDimensions(500, 50);
    bar2->setMaterialName("Core/StatsBlockCenter");
    bar2->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.7f)); // Fond noir semi-transparent
    
    // Créer la barre de remplissage d'énergie
    energyFill = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "EnergyFill"));
    energyFill->setMetricsMode(Ogre::GMM_PIXELS);
    energyFill->setPosition(0, 0); // Position relative au parent (bar2)
    energyFill->setDimensions(500, 50);
    energyFill->setMaterialName("Core/EnergyBar");
    
    // Ajouter les barres à l'overlay
    hudOverlay->add2D(bar1);
    hudOverlay->add2D(bar2);
    bar1->addChild(healthFill);
    bar2->addChild(energyFill);
}

void HUD::updateHealthBar(float currentHealth, float maxHealth) {
    if (!healthFill) return;
    
    float percentage = currentHealth / maxHealth;
    percentage = std::max(0.0f, std::min(1.0f, percentage)); // Limiter entre 0 et 1
    healthFill->setWidth(500 * percentage); // 500 est la largeur totale de la barre
}

void HUD::updateEnergyBar(float currentEnergy, float maxEnergy) {
    if (!energyFill) return;
    
    float percentage = currentEnergy / maxEnergy;
    percentage = std::max(0.0f, std::min(1.0f, percentage)); // Limiter entre 0 et 1
    energyFill->setWidth(500 * percentage); // 500 est la largeur totale de la barre
}

void HUD::show() {
    if (hudOverlay) hudOverlay->show();
}

void HUD::hide() {
    if (hudOverlay) hudOverlay->hide();
} 
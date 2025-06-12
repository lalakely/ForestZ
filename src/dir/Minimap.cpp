#include "../include/Minimap.hpp"

Minimap::Minimap() 
    : minimapOverlay(nullptr)
    , minimapBackground(nullptr)
    , playerIcon(nullptr)
{
}

Minimap::~Minimap() {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

    // Nettoyer les icônes des zombies
    for (auto* icon : zombieIcons) {
        if (icon) {
            overlayManager.destroyOverlayElement(icon);
        }
    }
    zombieIcons.clear();

    // Nettoyer les icônes des arbres
    for (auto* icon : treeIcons) {
        if (icon) {
            overlayManager.destroyOverlayElement(icon);
        }
    }
    treeIcons.clear();

    // Nettoyer les autres éléments
    if (playerIcon) {
        overlayManager.destroyOverlayElement(playerIcon);
    }
    if (minimapBackground) {
        overlayManager.destroyOverlayElement(minimapBackground);
    }
    if (minimapOverlay) {
        overlayManager.destroy(minimapOverlay);
    }
}

void Minimap::adjustPosition(unsigned int screenWidth) {
    if (minimapBackground) {
        minimapBackground->setPosition(screenWidth - MINIMAP_SIZE - MINIMAP_MARGIN, MINIMAP_MARGIN);
    }
}

void Minimap::initialize(Ogre::SceneManager* scnMgr) {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

    // Créer l'overlay principal
    minimapOverlay = overlayManager.create("MinimapOverlay");

    // Créer le fond du minimap
    minimapBackground = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "MinimapBackground"));
    minimapBackground->setMetricsMode(Ogre::GMM_PIXELS);
    
    // Position initiale (sera ajustée plus tard)
    minimapBackground->setPosition(800 - MINIMAP_SIZE - MINIMAP_MARGIN, MINIMAP_MARGIN);
    minimapBackground->setDimensions(MINIMAP_SIZE, MINIMAP_SIZE);
    minimapBackground->setMaterialName("Core/StatsBlockCenter");

    // Créer l'icône du joueur
    playerIcon = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", "PlayerIcon"));
    playerIcon->setMetricsMode(Ogre::GMM_PIXELS);
    playerIcon->setDimensions(6, 6);  // Taille de l'icône du joueur
    playerIcon->setMaterialName("Core/PlayerIcon");

    // Ajouter les éléments à l'overlay
    minimapBackground->addChild(playerIcon);
    minimapOverlay->add2D(minimapBackground);
    minimapOverlay->setZOrder(200);  // Au-dessus du jeu mais en-dessous des messages
    minimapOverlay->show();
}

void Minimap::createMinimapIcon(const std::string& name, const Ogre::ColourValue& color) {
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    
    auto* icon = static_cast<Ogre::OverlayContainer*>(
        overlayManager.createOverlayElement("Panel", name));
    icon->setMetricsMode(Ogre::GMM_PIXELS);
    icon->setDimensions(4, 4);  // Taille plus petite pour les autres icônes
    
    // Utiliser le matériau approprié en fonction du type d'icône
    if (name.find("Zombie") != std::string::npos) {
        icon->setMaterialName("Core/ZombieIcon");
    } else {
        // Pour les autres icônes (arbres, etc.), utiliser le matériau par défaut
        icon->setMaterialName("Core/StatsBlockCenter");
        // Convertir la couleur en string pour la bordure
        std::stringstream ss;
        ss << color.r << " " << color.g << " " << color.b;
        icon->setParameter("border_color", ss.str());
    }
    
    minimapBackground->addChild(icon);
}

void Minimap::updateIconPosition(const std::string& name, const Ogre::Vector3& worldPos) {
    auto* icon = static_cast<Ogre::OverlayContainer*>(
        Ogre::OverlayManager::getSingleton().getOverlayElement(name));
    if (!icon) return;

    // Si c'est l'icône du joueur, le placer au centre
    if (name == "PlayerIcon") {
        icon->setPosition((MINIMAP_SIZE / 2) - 3, (MINIMAP_SIZE / 2) - 3);  // -3 pour centrer l'icône de 6x6
        return;
    }

    // Pour les autres icônes, calculer leur position relative au joueur
    Ogre::Vector3 relativePos = worldPos - playerLastPosition;
    
    // Convertir la position relative en coordonnées de minimap
    float mapX = (MINIMAP_SIZE / 2) + (relativePos.x * MINIMAP_SCALE);
    float mapZ = (MINIMAP_SIZE / 2) + (relativePos.z * MINIMAP_SCALE);
    
    // S'assurer que l'icône reste dans les limites du minimap
    mapX = std::max(0.0f, std::min(mapX, MINIMAP_SIZE));
    mapZ = std::max(0.0f, std::min(mapZ, MINIMAP_SIZE));
    
    // Ajuster pour centrer l'icône (les icônes font 4x4 pixels)
    icon->setPosition(mapX - 2, mapZ - 2);
}

void Minimap::update(const Ogre::Vector3& playerPos,
                    const std::vector<Ogre::Vector3>& zombiePositions,
                    const std::vector<Ogre::Vector3>& treePositions) {
    // Mettre à jour la dernière position du joueur
    playerLastPosition = playerPos;

    // Mettre à jour la position du joueur (toujours au centre)
    updateIconPosition("PlayerIcon", playerPos);

    // Mettre à jour ou créer les icônes des zombies
    while (zombieIcons.size() < zombiePositions.size()) {
        std::string name = "ZombieIcon" + std::to_string(zombieIcons.size());
        createMinimapIcon(name, Ogre::ColourValue(1, 0, 0));  // Rouge pour les zombies
        zombieIcons.push_back(static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().getOverlayElement(name)));
    }

    // Mettre à jour les positions des zombies
    for (size_t i = 0; i < zombiePositions.size(); ++i) {
        updateIconPosition("ZombieIcon" + std::to_string(i), zombiePositions[i]);
    }

    // Mettre à jour ou créer les icônes des arbres
    while (treeIcons.size() < treePositions.size()) {
        std::string name = "TreeIcon" + std::to_string(treeIcons.size());
        createMinimapIcon(name, Ogre::ColourValue(0.5, 0.35, 0.05));  // Marron pour les arbres
        treeIcons.push_back(static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().getOverlayElement(name)));
    }

    // Mettre à jour les positions des arbres
    for (size_t i = 0; i < treePositions.size(); ++i) {
        updateIconPosition("TreeIcon" + std::to_string(i), treePositions[i]);
    }
} 
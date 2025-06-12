#ifndef ZOMBIES_HPP
#define ZOMBIES_HPP

#include <Ogre.h>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include "lib.hpp" // Include your lib.hpp for Ogre and Bullet includes
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

class Zombies {
public:
    Zombies();
    ~Zombies();

    void createZombies(Ogre::SceneManager* scnMgr, int numZombies, float radius, btDiscreteDynamicsWorld* dynamicsWorld);
    void updateZombies(Ogre::SceneNode* playerNode, float deltaTime);
    void onBulletHit(size_t zombieIndex, float damage, btDiscreteDynamicsWorld* dynamicsWorld);
    const std::vector<btRigidBody*>& getZombieBodies() const;
    const std::vector<Ogre::SceneNode*>& getZombieNodes() const { return zombieNodes; }
    bool isZombieAlive(size_t index) const;
    void setHealthMultiplier(float multiplier);
    void setSpeedMultiplier(float multiplier);
    
    // Nouvelles méthodes pour l'affichage à l'écran
    void showGameMessage(const std::string& message, float displayTime = 3.0f, int fontSize = 24);
    void showKillLogMessage(const std::string& message, float displayTime);
    void updateOverlay(float deltaTime);

private:
    std::vector<Ogre::SceneNode*> zombieNodes;
    std::vector<Ogre::Entity*> zombieEntities;
    std::vector<btRigidBody*> zombieBodies;
    std::vector<float> zombieHealth;
    float baseZombieHealth = 100.0f;
    float healthMultiplier = 1.0f;
    float speedMultiplier = 1.0f;

    // Système d'affichage
    Ogre::Overlay* gameOverlay;
    Ogre::OverlayContainer* overlayContainer;
    Ogre::TextAreaOverlayElement* messageText;
    float messageDisplayTimeRemaining;

    void initializeOverlay();
};

#endif // ZOMBIES_HPP
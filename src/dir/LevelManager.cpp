#include "../include/LevelManager.hpp"
#include <OgreTimer.h>

LevelManager::LevelManager(Ogre::SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld)
    : scnMgr(scnMgr),
      dynamicsWorld(dynamicsWorld),
      zombies(nullptr),
      player(nullptr),
      currentLevel(1),
      zombiesPerLevel(ZOMBIES_NUMBER),
      zombieHealthMultiplier(1.0f),
      zombieSpeedMultiplier(1.0f),
      levelTransitioning(false),
      levelTransitionTimer(0.0f)
{
}

LevelManager::~LevelManager()
{
    // Zombies and player are owned by Forest class, don't delete them here
}

void LevelManager::startNextLevel()
{
    currentLevel++;
    levelTransitioning = true;
    levelTransitionTimer = LEVEL_TRANSITION_TIME;

    // Increase difficulty progressively
    zombiesPerLevel = ZOMBIES_NUMBER + (currentLevel - 1) * 5;  // +5 zombies per level
    zombieHealthMultiplier = 1.0f + (currentLevel - 1) * 0.2f;  // +20% health per level
    zombieSpeedMultiplier = 1.0f + (currentLevel - 1) * 0.1f;   // +10% speed per level

    // Remove old zombies
    if (zombies) {
        delete zombies;
    }

    // Create new zombies
    zombies = new Zombies();
    if (zombies) {
        zombies->createZombies(scnMgr, zombiesPerLevel, 500, dynamicsWorld);
        // Apply multipliers
        zombies->setHealthMultiplier(zombieHealthMultiplier);
        zombies->setSpeedMultiplier(zombieSpeedMultiplier);
    }

    // Display level info
    displayLevelInfo();
}

void LevelManager::checkLevelCompletion()
{
    if (!zombies || levelTransitioning) return;

    bool allZombiesDead = true;
    const auto& zombieBodies = zombies->getZombieBodies();
    
    for (size_t i = 0; i < zombieBodies.size(); ++i) {
        if (zombies->isZombieAlive(i)) {
            allZombiesDead = false;
            break;
        }
    }

    if (allZombiesDead) {
        startNextLevel();
    }
}

void LevelManager::displayLevelInfo()
{
    if (!zombies) return;
    
    std::stringstream ss;
    ss << "LEVEL " << currentLevel;
    
    zombies->showGameMessage(ss.str(), 3.0f, 50);
}

void LevelManager::resetGame()
{
    // Reset level and multipliers
    currentLevel = 1;
    zombiesPerLevel = ZOMBIES_NUMBER;
    zombieHealthMultiplier = 1.0f;
    zombieSpeedMultiplier = 1.0f;

    // Reset player first
    if (player) {
        player->resetPlayer();
    }

    // Delete current zombies
    if (zombies) {
        // Show game over message
        zombies->showGameMessage("Game Over - Retour au niveau 1", 3.0f, 50);

        // Progressively remove all zombies
        const auto& zombieBodies = zombies->getZombieBodies();
        for (size_t i = 0; i < zombieBodies.size(); ++i) {
            if (zombies->isZombieAlive(i)) {
                zombies->onBulletHit(i, 1000.0f, dynamicsWorld); // Enough damage to kill instantly
            }
        }

        // Wait a short time for death animation
        Ogre::Timer timer;
        float elapsedTime = 0.0f;
        const float waitTime = 1.0f; // Wait 1 second

        while (elapsedTime < waitTime) {
            if (dynamicsWorld) {
                dynamicsWorld->stepSimulation(1.0f/60.0f);
            }
            elapsedTime = timer.getMilliseconds() / 1000.0f;
        }

        // Delete zombies instance completely
        delete zombies;
        zombies = new Zombies();
        if (zombies) {
            zombies->createZombies(scnMgr, zombiesPerLevel, 500, dynamicsWorld);
        }
    }
}

void LevelManager::update(float deltaTime)
{
    // Handle level transition
    if (levelTransitioning) {
        levelTransitionTimer -= deltaTime;
        if (levelTransitionTimer <= 0) {
            levelTransitioning = false;
            displayLevelInfo();
        }
    }
}

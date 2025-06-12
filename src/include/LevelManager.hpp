#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>
#include "../include/Zombies.hpp"
#include "../include/Player.hpp"
#include <sstream>

// Constantes (utiliser ZOMBIES_NUMBER de lib.hpp)
#define LEVEL_TRANSITION_TIME 3.0f

class LevelManager {
public:
    LevelManager(Ogre::SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    ~LevelManager();

    void startNextLevel();
    void checkLevelCompletion();
    void displayLevelInfo();
    void resetGame();
    
    // Getters
    int getCurrentLevel() const { return currentLevel; }
    int getZombiesPerLevel() const { return zombiesPerLevel; }
    float getZombieHealthMultiplier() const { return zombieHealthMultiplier; }
    float getZombieSpeedMultiplier() const { return zombieSpeedMultiplier; }
    bool isLevelTransitioning() const { return levelTransitioning; }
    
    // Setters
    void setZombies(Zombies* zombies) { this->zombies = zombies; }
    void setPlayer(Player* player) { this->player = player; }
    
    void update(float deltaTime);

private:
    Ogre::SceneManager* scnMgr;
    btDiscreteDynamicsWorld* dynamicsWorld;
    Zombies* zombies;
    Player* player;
    
    // Level system
    int currentLevel;
    int zombiesPerLevel;
    float zombieHealthMultiplier;
    float zombieSpeedMultiplier;
    bool levelTransitioning;
    float levelTransitionTimer;
};

#endif // LEVEL_MANAGER_HPP

#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreVector.h>  // Use OgreVector.h instead of deprecated OgreVector3.h
#include <btBulletDynamicsCommon.h>
#include "../include/Player.hpp"

// Pas besoin de forward declarations pour TrayManager

// Utilisation des constantes définies dans lib.hpp
#define PLAYER_SPRINT_MULTIPLIER 1.5f

class InputManager : public OgreBites::InputListener {
public:
    InputManager(btDiscreteDynamicsWorld* dynamicsWorld);
    ~InputManager();

    // Input event handlers
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    
    // Movement handling
    void updateMovementAndAnimation();
    
    // Setters
    void setPlayer(Player* player) { this->player = player; }
    void setGameStarted(bool started) { gameStarted = started; }
    void setWelcomePageActive(bool active) { welcomePageActive = active; }
    void setCameraNode(Ogre::SceneNode* node) { camNode = node; }
    void setSceneManager(Ogre::SceneManager* scnMgr) { this->scnMgr = scnMgr; }
    
    // Getters
    Ogre::Vector3 getDirection() const { return direction; }
    float getCurrentPitch() const { return currentPitch; }
    bool isKeyShiftPressed() const { return keyShiftPressed; }
    
    // Shooting
    Ogre::Vector3 getAimDirection() const;

private:
    Player* player;
    btDiscreteDynamicsWorld* dynamicsWorld;
    Ogre::SceneManager* scnMgr;
    Ogre::SceneNode* camNode;
    bool welcomePageActive;
    
    // Movement state
    Ogre::Vector3 direction;
    bool keyForwardPressed;
    bool keyBackwardPressed;
    bool keyLeftPressed;
    bool keyRightPressed;
    bool keyShiftPressed;
    
    // Camera control
    float currentPitch;
    float maxPitchAngle;
    
    // Game state
    bool gameStarted;
};

#endif // INPUT_MANAGER_HPP

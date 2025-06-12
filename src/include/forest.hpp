#pragma once

#include <exception>
#include <iostream>
#include <vector>
#include <memory>

#include "lib.hpp"
#include "Plane.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Zombies.hpp"
#include "CameraManager.hpp"
#include "LevelManager.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "UIManager.hpp"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>

using namespace Ogre;
using namespace OgreBites;
using namespace Ogre::RTShader;
using namespace std;

namespace Ogre {
    class OverlaySystem;
}

class Forest : public ApplicationContext, public InputListener
{
private:
    // Basic scene components
    SceneManager* scnMgr;
    Root* root;
    Object* object;
    PlaneZ* planeZ;
    Player* player;
    Zombies* zombies;
    OverlaySystem* overlaySystem;
    RTShader::ShaderGenerator* shadergen;
    float lastFrameTime;
    float lodDistanceThreshold;
    
    // Game state
    bool gameStarted;
    std::vector<btRigidBody*> testCubeBodies;
    
    // Manager classes
    CameraManager* cameraManager;
    LevelManager* levelManager;
    InputManager* inputManager;
    PhysicsManager* physicsManager;
    UIManager* uiManager;
    
    // Helper methods
    std::vector<Vector3> getTreePositions() const;
    std::vector<Vector3> getZombiePositions() const;

public:
    Forest();
    ~Forest() override;

    void setup() override;
    bool frameRenderingQueued(const FrameEvent& evt) override;

    // Input handling - delegate to InputManager
    bool keyPressed(const KeyboardEvent& evt) override;
    bool keyReleased(const KeyboardEvent& evt) override;
    bool mouseMoved(const MouseMotionEvent& evt) override;
    bool mousePressed(const MouseButtonEvent& evt) override;
    bool mouseReleased(const MouseButtonEvent& evt) override;

    // Game flow control
    void startGame();
    void quitGame();

private:
    // Lighting
    Light* light;
    SceneNode* lightNode;
    void createLight();
};
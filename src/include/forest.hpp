#ifndef FOREST_HPP
#define FOREST_HPP

#include <exception>
#include <iostream>
#include <vector>
#include <memory>

#include "lib.hpp"
#include "Plane.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Zombies.hpp"
#include "Minimap.hpp"
#include "HUD.hpp"
#include "Crosshair.hpp"
#include "BulletDebugDrawer.hpp"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreCameraMan.h>
#include <btBulletDynamicsCommon.h>

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
    // Camera control
    float currentPitch;
    float maxPitchAngle;
    float cameraSmoothTime;
    float maxCameraSpeed;
    Vector3 cameraOffset;
    Vector3 currentCameraVelocity;
    Vector3 targetCameraPosition;

    // Basic scene components
    SceneManager* scnMgr;
    Root* root;
    Camera* cam;
    SceneNode* camNode;
    Light* light;
    SceneNode* lightNode;
    Viewport* vp;
    OgreBites::CameraMan* cameraMan;
    Object* object;
    PlaneZ* planeZ;
    Player* player;
    Zombies* zombies;
    Minimap* minimap;
    HUD* hud;
    Crosshair* crosshair;
    OverlaySystem* overlaySystem;
    RTShader::ShaderGenerator* shadergen;
    Vector3 direction;
    float lastFrameTime;
    float lodDistanceThreshold;

    // Movement key states
    bool keyForwardPressed;
    bool keyBackwardPressed;
    bool keyLeftPressed;
    bool keyRightPressed;
    bool keyShiftPressed;

    // Physics
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    BulletDebugDrawer* debugDrawer;
    std::vector<btRigidBody*> testCubeBodies;

    // Level system
    int currentLevel;
    int zombiesPerLevel;
    float zombieHealthMultiplier;
    float zombieSpeedMultiplier;
    bool levelTransitioning;
    float levelTransitionTimer;

    // Helper methods
    std::vector<Vector3> getTreePositions() const;
    std::vector<Vector3> getZombiePositions() const;
    Vector3 getAimDirection() const;

public:
    Forest();
    virtual ~Forest();

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void createLight();
    void createCam();
    void moveCamera(const OgreBites::KeyboardEvent& evt);
    void updateMovementAndAnimation();
    void updateCameraPosition();
    void renderDebugLines();
    Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity, 
                      float smoothTime, float deltaTime, float maxSpeed);
    void startNextLevel();
    void checkLevelCompletion();
    void displayLevelInfo();
    void resetGame();
};

#endif
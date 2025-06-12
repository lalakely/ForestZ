#ifndef FOREST_HPP
#define FOREST_HPP

#include "lib.hpp"
#include "Object.hpp"
#include "Plane.hpp"
#include "Player.hpp"
#include "Zombies.hpp"
#include "HUD.hpp"
#include "Minimap.hpp"
#include "Crosshair.hpp"
#include <memory>
#include <vector>

#define LEVEL_TRANSITION_TIME 3.0f // Temps de transition entre les niveaux en secondes

// Forward declaration
class PlaneZ;

class Forest : public ApplicationContext, public InputListener
{
private:
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

    // Camera control
    float currentPitch;
    float maxPitchAngle;
    float cameraSmoothTime;
    float maxCameraSpeed;
    Vector3 cameraOffset;
    Vector3 currentCameraVelocity;
    Vector3 targetCameraPosition;

    // Helper methods
    void updateMovementAndAnimation();
    std::vector<Vector3> getTreePositions() const;
    std::vector<Vector3> getZombiePositions() const;

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
    void updateCameraPosition();
    void renderDebugLines();
    Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity, 
                      float smoothTime, float deltaTime, float maxSpeed);
    void startNextLevel();
    void checkLevelCompletion();
    void displayLevelInfo();
    void resetGame();
    Vector3 getAimDirection() const;
};

#endif 
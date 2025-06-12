#ifndef PHYSICS_MANAGER_HPP
#define PHYSICS_MANAGER_HPP

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <OgreSceneManager.h>
#include "BulletDebugDrawer.hpp"

// Custom callback class for collision detection
class CustomContactResultCallback : public btCollisionWorld::ContactResultCallback {
public:
    bool collisionDetected = false;

    // Override the addSingleResult method
    btScalar addSingleResult(btManifoldPoint& cp,
                             const btCollisionObjectWrapper* colObj0Wrap,
                             int partId0, int index0,
                             const btCollisionObjectWrapper* colObj1Wrap,
                             int partId1, int index1) override {
        collisionDetected = true; // Set flag to true if a collision is detected
        return 0; // Return value is not used
    }
};

class PhysicsManager {
public:
    PhysicsManager();
    ~PhysicsManager();

    void initialize();
    void setupDebugDrawer(Ogre::SceneManager* scnMgr);
    void renderDebugLines();
    void stepSimulation(float deltaTime);
    
    // Collision detection
    bool checkCollision(btCollisionObject* obj1, btCollisionObject* obj2);
    
    // Getters
    btDiscreteDynamicsWorld* getDynamicsWorld() const { return dynamicsWorld; }

private:
    // Bullet Physics components
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btDbvtBroadphase> broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    BulletDebugDrawer* debugDrawer;
};

#endif // PHYSICS_MANAGER_HPP

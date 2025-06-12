#include "../include/PhysicsManager.hpp"

PhysicsManager::PhysicsManager()
    : dynamicsWorld(nullptr),
      debugDrawer(nullptr)
{
}

PhysicsManager::~PhysicsManager()
{
    delete debugDrawer;
    delete dynamicsWorld;
}

void PhysicsManager::initialize()
{
    collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfig.get());
    broadphase = std::make_unique<btDbvtBroadphase>();
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    
    dynamicsWorld = new btDiscreteDynamicsWorld(
        dispatcher.get(), 
        broadphase.get(), 
        solver.get(), 
        collisionConfig.get()
    );
    
    if (dynamicsWorld) {
        dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
    }
}

void PhysicsManager::setupDebugDrawer(Ogre::SceneManager* scnMgr)
{
    if (!dynamicsWorld) return;
    
    debugDrawer = new BulletDebugDrawer(scnMgr);
    if (debugDrawer) {
        debugDrawer->setDebugMode(
            btIDebugDraw::DBG_DrawWireframe | 
            btIDebugDraw::DBG_DrawAabb | 
            btIDebugDraw::DBG_DrawContactPoints
        );
        dynamicsWorld->setDebugDrawer(debugDrawer);
    }
}

void PhysicsManager::renderDebugLines()
{
    if (debugDrawer && dynamicsWorld) {
        debugDrawer->begin();
        dynamicsWorld->debugDrawWorld();
        debugDrawer->end();
    }
}

void PhysicsManager::stepSimulation(float deltaTime)
{
    if (dynamicsWorld) {
        dynamicsWorld->stepSimulation(deltaTime, 10);
    }
}

bool PhysicsManager::checkCollision(btCollisionObject* obj1, btCollisionObject* obj2)
{
    if (!dynamicsWorld || !obj1 || !obj2) return false;
    
    CustomContactResultCallback callback;
    dynamicsWorld->contactPairTest(obj1, obj2, callback);
    
    return callback.collisionDetected;
}

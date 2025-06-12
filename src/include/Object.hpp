#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include "lib.hpp"

using namespace Ogre;

/**
 * @class Object
 * @brief Manages the creation and management of environmental objects in the game world
 * 
 * This class is responsible for creating and managing objects like trees and walls
 * in the game world, including both their visual representation and physics properties.
 * It implements a Level of Detail (LOD) system for optimizing rendering performance.
 */
class Object {
public:
    /**
     * @brief Default constructor
     */
    Object();

    /**
     * @brief Destructor that cleans up all resources
     */
    ~Object();

    /**
     * @brief Creates all objects in the scene
     * @param scnMgr Pointer to the scene manager
     * @param dynamicsWorld Pointer to the physics world
     */
    void createObject(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);

    /**
     * @brief Updates the Level of Detail for objects based on camera distance
     * @param camNode Pointer to the camera node
     * @param scnMgr Pointer to the scene manager
     */
    void updateObjectLODs(SceneNode* camNode, SceneManager* scnMgr);

    /**
     * @brief Renders debug information for physics objects
     * @param debugDrawer Pointer to the debug drawer
     * @param dynamicsWorld Pointer to the physics world
     */
    void renderDebug(btIDebugDraw* debugDrawer, btDiscreteDynamicsWorld* dynamicsWorld);

    /**
     * @brief Gets the vector of tree nodes
     * @return Constant reference to the vector of tree nodes
     */
    const std::vector<Ogre::SceneNode*>& getTreeNodes() const { return treeNodes; }

    /**
     * @brief Gets the vector of rigid bodies
     * @return Constant reference to the vector of rigid bodies
     */
    const std::vector<btRigidBody*>& getRigidBodies() const { return treeBodies; }

    /**
     * @brief Gets the vector of collision shapes
     * @return Constant reference to the vector of collision shapes
     */
    const std::vector<btCollisionShape*>& getCollisionShapes() const { return treeShapes; }

    /**
     * @brief Gets the vector of motion states
     * @return Constant reference to the vector of motion states
     */
    const std::vector<btDefaultMotionState*>& getMotionStates() const { return treeMotionStates; }

private:
    // Member variables
    std::vector<btRigidBody*> treeBodies;
    std::vector<btCollisionShape*> treeShapes;
    std::vector<btDefaultMotionState*> treeMotionStates;
    std::vector<SceneNode*> treeNodes;
    MaterialPtr material;
    float lodDistanceThreshold = 500.0f;

    // Helper methods
    void cleanupPhysicsResources();
    void createBoundaryWalls(btDiscreteDynamicsWorld* dynamicsWorld);
    void createWall(const btVector3& size, const btVector3& position, btDiscreteDynamicsWorld* dynamicsWorld);
    void createBoundaryTrees(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    void createRandomTrees(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    void createTreeAtPosition(float x, float z, SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);
    void createTreePhysics(float x, float z, btDiscreteDynamicsWorld* dynamicsWorld);
    void updateTreeLOD(size_t index, const Vector3& cameraPosition, 
                      const std::vector<std::string>& treeModels,
                      const std::vector<std::string>& materials,
                      SceneManager* scnMgr);
    void switchToHighDetailTree(size_t index, SceneNode* treeNode,
                              const std::vector<std::string>& treeModels,
                              const std::vector<std::string>& materials,
                              SceneManager* scnMgr);
    void switchToLowDetailTree(size_t index, SceneNode* treeNode, SceneManager* scnMgr);
    void setupTreeMaterial(const std::string& materialName);
    std::string generateUniqueId();
};

#endif
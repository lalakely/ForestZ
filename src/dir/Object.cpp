#include "../include/Object.hpp"

/**
 * @brief Destructor that properly cleans up all physics-related resources
 */
Object::~Object() {
    cleanupPhysicsResources();
}

/**
 * @brief Default constructor
 */
Object::Object() {}

/**
 * @brief Cleans up all physics-related resources
 */
void Object::cleanupPhysicsResources() {
    // Clean up rigid bodies and their motion states
    for (auto body : treeBodies) {
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        delete body;
    }

    // Clean up collision shapes
    for (auto shape : treeShapes) {
        delete shape;
    }

    // Clear all vectors
    treeBodies.clear();
    treeShapes.clear();
    treeMotionStates.clear();
}

/**
 * @brief Creates boundary walls and trees in the scene
 * @param scnMgr Pointer to the scene manager
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createObject(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld) {
    createBoundaryWalls(dynamicsWorld);
    createBoundaryTrees(scnMgr, dynamicsWorld);
    createRandomTrees(scnMgr, dynamicsWorld);
}

/**
 * @brief Creates invisible boundary walls for the map
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createBoundaryWalls(btDiscreteDynamicsWorld* dynamicsWorld) {
    const float mapWidth = PLANE_WIDTH;
    const float mapHeight = PLANE_HEIGHT;
    const float margin = 200.0f;
    const float halfWidth = (mapWidth / 2.0f) - margin;
    const float halfHeight = (mapHeight / 2.0f) - margin;
    const float wallHeight = 200.0f;

    // Create wall specifications
    struct WallSpec {
        btVector3 size;
        btVector3 position;
    };

    std::vector<WallSpec> walls = {
        // North wall
        {{halfWidth, wallHeight, 5.0f}, {0, wallHeight, halfHeight}},
        // South wall
        {{halfWidth, wallHeight, 5.0f}, {0, wallHeight, -halfHeight}},
        // East wall
        {{5.0f, wallHeight, halfHeight}, {halfWidth, wallHeight, 0}},
        // West wall
        {{5.0f, wallHeight, halfHeight}, {-halfWidth, wallHeight, 0}}
    };

    // Create all walls
    for (const auto& wall : walls) {
        createWall(wall.size, wall.position, dynamicsWorld);
    }
}

/**
 * @brief Creates a single boundary wall
 * @param size Size of the wall
 * @param position Position of the wall
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createWall(const btVector3& size, const btVector3& position, btDiscreteDynamicsWorld* dynamicsWorld) {
    btCollisionShape* wallShape = new btBoxShape(size);
    btTransform wallTransform;
    wallTransform.setIdentity();
    wallTransform.setOrigin(position);
    
    btDefaultMotionState* wallMotionState = new btDefaultMotionState(wallTransform);
    btRigidBody::btRigidBodyConstructionInfo wallRbInfo(0, wallMotionState, wallShape);
    btRigidBody* wallBody = new btRigidBody(wallRbInfo);
    
    dynamicsWorld->addRigidBody(wallBody);
    
    // Store for cleanup
    treeBodies.push_back(wallBody);
    treeShapes.push_back(wallShape);
    treeMotionStates.push_back(wallMotionState);
}

/**
 * @brief Creates trees along the boundary of the map
 * @param scnMgr Pointer to the scene manager
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createBoundaryTrees(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld) {
    const float treeSpacing = 80.0f;
    const float halfWidth = (PLANE_WIDTH / 2.0f) - 200.0f;
    const float halfHeight = (PLANE_HEIGHT / 2.0f) - 200.0f;

    // Create trees along North and South boundaries
    for (float x = -halfWidth + treeSpacing; x < halfWidth; x += treeSpacing) {
        createTreeAtPosition(x, halfHeight, scnMgr, dynamicsWorld);
        createTreeAtPosition(x, -halfHeight, scnMgr, dynamicsWorld);
    }

    // Create trees along East and West boundaries
    for (float z = -halfHeight + treeSpacing; z < halfHeight; z += treeSpacing) {
        createTreeAtPosition(halfWidth, z, scnMgr, dynamicsWorld);
        createTreeAtPosition(-halfWidth, z, scnMgr, dynamicsWorld);
    }
}

/**
 * @brief Creates random trees throughout the map
 * @param scnMgr Pointer to the scene manager
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createRandomTrees(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld) {
    std::srand(static_cast<unsigned>(std::time(0)));
    
    for (int i = 0; i < TREE_NUMBER; ++i) {
        float x = static_cast<float>(std::rand()) / RAND_MAX * PLANE_WIDTH - PLANE_WIDTH / 2;
        float z = static_cast<float>(std::rand()) / RAND_MAX * PLANE_HEIGHT - PLANE_HEIGHT / 2;
        
        createTreeAtPosition(x, z, scnMgr, dynamicsWorld);
    }
}

/**
 * @brief Creates a tree at the specified position
 * @param x X coordinate
 * @param z Z coordinate
 * @param scnMgr Pointer to the scene manager
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createTreeAtPosition(float x, float z, SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld) {
    // Create visual representation
    std::string entityName = "tree_placeholder_" + generateUniqueId();
    Entity* placeholder = scnMgr->createEntity(entityName, Ogre::SceneManager::PT_CUBE);
    SceneNode* treeNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    treeNode->attachObject(placeholder);
    treeNode->setPosition(Vector3(x, 0, z));
    treeNode->setScale(0.1f, 0.1f, 0.1f);
    treeNodes.push_back(treeNode);

    // Create physics representation
    createTreePhysics(x, z, dynamicsWorld);
}

/**
 * @brief Creates the physics representation of a tree
 * @param x X coordinate
 * @param z Z coordinate
 * @param dynamicsWorld Pointer to the physics world
 */
void Object::createTreePhysics(float x, float z, btDiscreteDynamicsWorld* dynamicsWorld) {
    btCollisionShape* treeShape = new btBoxShape(btVector3(20.0f, 70.0f, 20.0f));
    treeShapes.push_back(treeShape);

    btTransform treeTransform;
    treeTransform.setIdentity();
    treeTransform.setOrigin(btVector3(x, 0, z));

    btDefaultMotionState* motionState = new btDefaultMotionState(treeTransform);
    treeMotionStates.push_back(motionState);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, treeShape, btVector3(0, 0, 0));
    btRigidBody* treeBody = new btRigidBody(rbInfo);
    treeBodies.push_back(treeBody);

    dynamicsWorld->addRigidBody(treeBody);
}

/**
 * @brief Updates the Level of Detail (LOD) for trees based on camera distance
 * @param camNode Camera node
 * @param scnMgr Scene manager
 */
void Object::updateObjectLODs(SceneNode* camNode, SceneManager* scnMgr) {
    Vector3 cameraPosition = camNode->getPosition();
    const std::vector<std::string> treeModels = {"tree_1.mesh", "tree_2.mesh"};
    const std::vector<std::string> materials = {"MT01_MatTreeF4_M6_P1", "MT01_MatTreeF4_M5_P1"};

    for (size_t i = 0; i < treeNodes.size(); ++i) {
        updateTreeLOD(i, cameraPosition, treeModels, materials, scnMgr);
    }
}

/**
 * @brief Updates the LOD for a single tree
 * @param index Tree index
 * @param cameraPosition Camera position
 * @param treeModels Available tree models
 * @param materials Available materials
 * @param scnMgr Scene manager
 */
void Object::updateTreeLOD(size_t index, const Vector3& cameraPosition, 
                          const std::vector<std::string>& treeModels,
                          const std::vector<std::string>& materials,
                          SceneManager* scnMgr) {
    SceneNode* treeNode = treeNodes[index];
    Vector3 treePosition = treeNode->getPosition();
    float distance = cameraPosition.distance(treePosition) - DISTANCE_RENDER_TREE;

    if (distance < lodDistanceThreshold && treeNode->numAttachedObjects() == 1) {
        switchToHighDetailTree(index, treeNode, treeModels, materials, scnMgr);
    } else if (distance >= lodDistanceThreshold && treeNode->numAttachedObjects() > 0) {
        switchToLowDetailTree(index, treeNode, scnMgr);
    }
}

/**
 * @brief Switches a tree to high detail representation
 */
void Object::switchToHighDetailTree(size_t index, SceneNode* treeNode,
                                  const std::vector<std::string>& treeModels,
                                  const std::vector<std::string>& materials,
                                  SceneManager* scnMgr) {
    treeNode->detachAllObjects();
    std::string treeName = "tree_full_" + generateUniqueId();
    Entity* tree = scnMgr->createEntity(treeName, treeModels[index % treeModels.size()]);
    tree->setCastShadows(true);
    
    std::string materialName = materials[index % materials.size()];
    tree->setMaterialName(materialName);
    
    setupTreeMaterial(materialName);
    
    treeNode->attachObject(tree);
    treeNode->setScale(0.1f, 0.1f, 0.1f);
}

/**
 * @brief Switches a tree to low detail representation
 */
void Object::switchToLowDetailTree(size_t index, SceneNode* treeNode, SceneManager* scnMgr) {
    treeNode->detachAllObjects();
    std::string placeholderName = "object_placeholder_" + generateUniqueId();
    Entity* placeholder = scnMgr->createEntity(placeholderName, Ogre::SceneManager::PT_CUBE);
    treeNode->attachObject(placeholder);
    treeNode->setScale(0.1f, 0.1f, 0.1f);
}

/**
 * @brief Sets up the material properties for a tree
 * @param materialName Name of the material to setup
 */
void Object::setupTreeMaterial(const std::string& materialName) {
    material = Ogre::MaterialManager::getSingleton().getByName(materialName);
    if (material) {
        material->setDiffuse(0.1f, 0.1f, 0.1f, 1.0f);
        material->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
        material->setAmbient(0.0f, 0.0f, 0.0f);
        material->setSelfIllumination(0.0f, 0.0f, 0.0f);
    }
}

/**
 * @brief Generates a truly unique identifier for entity names
 * @return Unique string identifier
 */
std::string Object::generateUniqueId() {
    static unsigned long counter = 0;
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    unsigned long timestamp = value.count();
    
    return std::to_string(timestamp) + "_" + 
           std::to_string(++counter) + "_" + 
           std::to_string(std::rand());
}

/**
 * @brief Renders debug information for physics objects
 * @param debugDrawer Debug drawer instance
 * @param dynamicsWorld Physics world instance
 */
void Object::renderDebug(btIDebugDraw* debugDrawer, btDiscreteDynamicsWorld* dynamicsWorld) {
    if (debugDrawer && dynamicsWorld) {
        dynamicsWorld->debugDrawWorld();
    }
}
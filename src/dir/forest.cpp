#include "../include/forest.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <OgreOverlaySystem.h>
#include <SDL2/SDL_keycode.h>
#include <Ogre.h>

Forest::Forest()
    : ApplicationContext("Zforest"), 
      scnMgr(nullptr),
      root(nullptr),
      object(nullptr),
      planeZ(nullptr),
      player(nullptr),
      zombies(nullptr),
      overlaySystem(nullptr),
      shadergen(nullptr),
      lastFrameTime(0.0f),
      gameStarted(false),
      cameraManager(nullptr),
      levelManager(nullptr),
      inputManager(nullptr),
      physicsManager(nullptr),
      uiManager(nullptr),
      light(nullptr),
      lightNode(nullptr)
{
}

Forest::~Forest()
{
    // Clean up managers
    delete uiManager;
    delete physicsManager;
    delete inputManager;
    delete levelManager;
    delete cameraManager;

    // Clean up game objects
    delete zombies;
    delete player;
    delete planeZ;
    delete object;
    delete overlaySystem;
}

void Forest::setup()
{
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    root = getRoot();
    scnMgr = root->createSceneManager();

    // Initialize resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- Overlay System --
    overlaySystem = getOverlaySystem();
    scnMgr->addRenderQueueListener(overlaySystem);

    // -- Create Camera --
    SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    Camera* cam = scnMgr->createCamera("MainCam");
    
    cam->setNearClipDistance(5);
    cam->setAutoAspectRatio(true);
    camNode->setPosition(0, 10, 50);
    camNode->lookAt(Vector3(0, 0, 0), Node::TS_WORLD);
    camNode->attachObject(cam);

    // -- Set Viewport --
    Viewport* vp = getRenderWindow()->addViewport(cam);
    vp->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f));

    // Create ground plane
    Plane plane(Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane(
        "ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 20, 20,
        true, 1, 5, 5,
        Vector3::UNIT_Z
    );
    Entity* groundEntity = scnMgr->createEntity("ground");
    SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    groundNode->attachObject(groundEntity);
    groundEntity->setMaterialName("ForestGround");

    // Create a test entity
    Entity* testEntity = scnMgr->createEntity("Mesh.mesh");
    SceneNode* testNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    testNode->attachObject(testEntity);
    testNode->setPosition(0, 0, 0);

    // Initialize managers
    physicsManager = new PhysicsManager();
    physicsManager->initialize();
    physicsManager->setupDebugDrawer(scnMgr);
    
    cameraManager = new CameraManager(scnMgr, getRenderWindow());
    
    uiManager = new UIManager(scnMgr, getRenderWindow());
    uiManager->initialize();
    
    levelManager = new LevelManager(scnMgr, physicsManager->getDynamicsWorld());
    
    inputManager = new InputManager(physicsManager->getDynamicsWorld());
    
    // Set up UI callbacks
    uiManager->setPlayCallback([this]() { this->startGame(); });
    uiManager->setQuitCallback([this]() { this->quitGame(); });
    
    if (uiManager->getTrayManager()) {
        addInputListener(uiManager->getTrayManager());
    }

    createLight();
}

void Forest::startGame()
{
    if (uiManager) {
        uiManager->hideWelcomePage();
    }
    
    gameStarted = true;
    
    btDiscreteDynamicsWorld* dynamicsWorld = physicsManager ? physicsManager->getDynamicsWorld() : nullptr;
    if (!dynamicsWorld) return;
    
    planeZ = new PlaneZ();
    planeZ->createPlane(scnMgr, dynamicsWorld);
    
    object = new Object();
    object->createObject(scnMgr, dynamicsWorld);
    
    player = new Player();
    player->createPlayer(scnMgr, Ogre::Vector3::ZERO, dynamicsWorld);
}

void Forest::quitGame()
{
    getRoot()->queueEndRendering();
}

bool Forest::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (!gameStarted) return true;

    if (inputManager) {
        inputManager->keyPressed(evt);
    }

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    return true;
}

bool Forest::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (!gameStarted) return true;
    if (inputManager) {
        inputManager->keyReleased(evt);
    }
    return true;
}

bool Forest::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (!gameStarted) return true;
    if (inputManager) {
        inputManager->mouseMoved(evt);
    }
    return true;
}

bool Forest::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    if (!gameStarted) return true;
    if (inputManager) {
        inputManager->mousePressed(evt);
    }
    return true;
}

bool Forest::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    if (!gameStarted) {
        if (uiManager && uiManager->getTrayManager()) {
            return uiManager->getTrayManager()->mouseReleased(evt);
        }
        return true;
    }
    if (inputManager) {
        inputManager->mouseReleased(evt);
    }
    return true;
}

void Forest::createLight()
{
    // Lumière ambiante
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    // Lumière directionnelle
    light = scnMgr->createLight("MainLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDiffuseColour(ColourValue(1.0, 1.0, 1.0));
    light->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
    
    lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setDirection(Vector3(0.5, -1, 0.5).normalisedCopy());
}

bool Forest::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (!gameStarted) {
        if (uiManager && uiManager->getTrayManager()) {
            uiManager->getTrayManager()->frameRendered(evt);
        }
        return true;
    }

    if (physicsManager) {
        physicsManager->stepSimulation(evt.timeSinceLastFrame);
    }

    if (cameraManager && player && player->playerNode) {
        cameraManager->updateCameraPosition(player->playerNode);
    }

    return true;
}

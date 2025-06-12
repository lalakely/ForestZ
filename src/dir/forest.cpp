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

    root = getRoot();
    overlaySystem = getOverlaySystem();
    scnMgr = root->createSceneManager();
    
    if (overlaySystem)
        scnMgr->addRenderQueueListener(overlaySystem);

    // Initialize managers
    physicsManager = new PhysicsManager();
    physicsManager->initialize();
    physicsManager->setupDebugDrawer(scnMgr);
    
    cameraManager = new CameraManager(scnMgr, getRenderWindow());
    
    uiManager = new UIManager(scnMgr, getRenderWindow(), overlaySystem);
    uiManager->initialize();
    
    levelManager = new LevelManager(scnMgr, physicsManager->getDynamicsWorld());
    
    inputManager = new InputManager();
    
    // Set up UI callbacks
    uiManager->setPlayCallback([this]() { this->startGame(); });
    uiManager->setMultiplayerCallback([this]() { 
        // TODO: Implémenter le mode multijoueur
        std::cout << "Mode multijoueur pas encore implémenté" << std::endl;
    });
    uiManager->setSettingsCallback([this]() {
        // TODO: Implémenter la page des paramètres
        std::cout << "Page des paramètres pas encore implémentée" << std::endl;
    });
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
    light = scnMgr->createLight("MainLight");
    lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    
    light->setDiffuseColour(Ogre::ColourValue(0.8f, 0.6f, 0.4f));
    light->setSpecularColour(Ogre::ColourValue(0.5f, 0.4f, 0.3f));
    light->setPowerScale(2.0f);

    lightNode->attachObject(light);
    lightNode->setPosition(Ogre::Vector3(500, 600, 500));
    lightNode->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
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

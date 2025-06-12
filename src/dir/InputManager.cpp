#include "../include/InputManager.hpp"
#include <iostream>

using namespace Ogre;

InputManager::InputManager(btDiscreteDynamicsWorld* dynamicsWorld)
    : player(nullptr),
      dynamicsWorld(dynamicsWorld),
      scnMgr(nullptr),
      camNode(nullptr),
      welcomePageActive(false),
      direction(Vector3::ZERO),
      keyForwardPressed(false),
      keyBackwardPressed(false),
      keyLeftPressed(false),
      keyRightPressed(false),
      keyShiftPressed(false),
      currentPitch(0.0f),
      maxPitchAngle(60.0f),
      gameStarted(false)
{
}

InputManager::~InputManager()
{
    // No resources to clean up
}

bool InputManager::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (!gameStarted) {
        // Si nous sommes sur la page d'accueil, laisser UIManager gérer les événements
        if (welcomePageActive) {
            // Simplement retourner true pour indiquer que l'événement a été traité
            return true;
        }
        return true;
    }

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        return false; // Let Forest handle this to quit the game
    }

    // Update key states
    switch (evt.keysym.sym) {
        case OgreBites::SDLK_UP:
        case 'z':
        case 'Z':
            keyForwardPressed = true;
            break;
        case OgreBites::SDLK_DOWN:
        case 's':
        case 'S':
            keyBackwardPressed = true;
            break;
        case OgreBites::SDLK_LEFT:
        case 'q':
        case 'Q':
            keyLeftPressed = true;
            break;
        case OgreBites::SDLK_RIGHT:
        case 'd':
        case 'D':
            keyRightPressed = true;
            break;
        case OgreBites::SDLK_LSHIFT:
            keyShiftPressed = true;
            break;
    }

    updateMovementAndAnimation();
    return true;
}

bool InputManager::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    // Update key states
    switch (evt.keysym.sym) {
        case OgreBites::SDLK_UP:
        case 'z':
        case 'Z':
            keyForwardPressed = false;
            break;
        case OgreBites::SDLK_DOWN:
        case 's':
        case 'S':
            keyBackwardPressed = false;
            break;
        case OgreBites::SDLK_LEFT:
        case 'q':
        case 'Q':
            keyLeftPressed = false;
            break;
        case OgreBites::SDLK_RIGHT:
        case 'd':
        case 'D':
            keyRightPressed = false;
            break;
        case OgreBites::SDLK_LSHIFT:
            keyShiftPressed = false;
            break;
    }

    updateMovementAndAnimation();
    return true;
}

void InputManager::updateMovementAndAnimation()
{
    if (!player || !player->playerNode || !player->playerBody) return;

    float moveSpeed = PLAYER_SPEED;
    if (keyShiftPressed) {
        moveSpeed *= PLAYER_SPRINT_MULTIPLIER;
    }

    direction = Vector3::ZERO;
    std::string targetAnimation = "Idle";
    bool isMoving = false;

    // Use player orientation for movement
    if (keyForwardPressed) {
        direction = player->playerNode->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
        isMoving = true;
        targetAnimation = keyShiftPressed ? "runForward" : "walkForward";
    }
    else if (keyBackwardPressed) {
        direction = player->playerNode->getOrientation() * Vector3::UNIT_Z;
        isMoving = true;
        targetAnimation = keyShiftPressed ? "runBackward" : "walkBackward";
    }
    else if (keyLeftPressed) {
        direction = player->playerNode->getOrientation() * Vector3::UNIT_X;
        isMoving = true;
        targetAnimation = keyShiftPressed ? "runLeft" : "walkLeft";
    }
    else if (keyRightPressed) {
        direction = player->playerNode->getOrientation() * Vector3::NEGATIVE_UNIT_X;
        isMoving = true;
        targetAnimation = keyShiftPressed ? "runRight" : "walkRight";
    }

    // Normalize direction for constant speed
    if (direction != Vector3::ZERO) {
        direction.normalise();
    }

    // Get current velocity
    btVector3 currentVel = player->playerBody->getLinearVelocity();
    
    // Calculate new velocity
    btVector3 targetVelocity(direction.x * moveSpeed, 0, direction.z * moveSpeed);
    
    // Calculate force needed to reach target velocity
    btVector3 velocityChange = targetVelocity - currentVel;
    btVector3 force = velocityChange * player->playerBody->getMass() * 10;
    
    // Apply force to center of mass
    player->playerBody->applyCentralForce(force);
    
    // Limit max speed
    if (currentVel.length() > moveSpeed) {
        currentVel = currentVel.normalized() * moveSpeed;
        player->playerBody->setLinearVelocity(currentVel);
    }

    // If no keys pressed, apply stronger braking
    if (!isMoving) {
        currentVel = player->playerBody->getLinearVelocity();
        player->playerBody->setLinearVelocity(currentVel * 0.8f);
    }

    // Set the target animation
    player->setAnimation(targetAnimation);

    player->playerBody->activate();
}

bool InputManager::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (!player || !player->playerNode || !camNode) return true;

    // Rotate player based on horizontal mouse movement (yaw)
    const float rotationSpeed = 0.06f;
    Radian yaw(evt.xrel * rotationSpeed);
    player->playerNode->yaw(yaw, Node::TS_WORLD);

    // Update camera pitch (vertical rotation)
    const float pitchSpeed = 0.3f;
    float pitchChange = -evt.yrel * pitchSpeed;
    
    // Update and clamp the pitch angle
    currentPitch += pitchChange;
    currentPitch = std::max(-maxPitchAngle, std::min(maxPitchAngle, currentPitch));

    return true;
}

bool InputManager::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    if (!gameStarted) {
        // Si nous sommes sur la page d'accueil, laisser UIManager gérer les événements
        if (welcomePageActive) {
            // Simplement retourner true pour indiquer que l'événement a été traité
            return true;
        }
        return true;
    }

    if (evt.button == OgreBites::BUTTON_LEFT) {
        if (player) {
            Vector3 shootDir = getAimDirection();
            player->shoot(scnMgr, dynamicsWorld, shootDir);
        }
    }
    
    return true;
}

bool InputManager::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    if (!gameStarted) {
        // Si nous sommes sur la page d'accueil, laisser UIManager gérer les événements
        if (welcomePageActive) {
            // Simplement retourner true pour indiquer que l'événement a été traité
            return true;
        }
        return true;
    }

    return true;
}

Vector3 InputManager::getAimDirection() const
{
    if (!camNode || !player || !player->playerNode) return Vector3::ZERO;

    Vector3 cameraDirection = camNode->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
    cameraDirection.normalise();
    
    return cameraDirection;
}

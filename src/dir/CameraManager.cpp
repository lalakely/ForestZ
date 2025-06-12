#include "../include/CameraManager.hpp"

CameraManager::CameraManager(SceneManager* scnMgr, RenderWindow* window)
    : scnMgr(scnMgr),
      window(window),
      cam(nullptr),
      camNode(nullptr),
      vp(nullptr),
      currentCameraVelocity(Vector3::ZERO),
      currentPitch(0.0f),
      maxPitchAngle(60.0f),
      cameraSmoothTime(0.3f),
      maxCameraSpeed(300.0f),
      cameraOffset(Vector3(-30, 100, -150))
{
}

CameraManager::~CameraManager()
{
    // Camera and viewport are managed by OGRE, no need to delete them
}

void CameraManager::createCamera()
{
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cam = scnMgr->createCamera("myCam");
    
    if (cam) {
        cam->setNearClipDistance(5);
        camNode->attachObject(cam);
        
        // Create viewport
        vp = window->addViewport(cam);
        if (vp) {
            vp->setBackgroundColour(ColourValue(0, 0, 0));
        }
    }
}

void CameraManager::setPitch(float pitch)
{
    currentPitch = pitch;
    // Clamp pitch to avoid camera flipping
    currentPitch = std::max(-maxPitchAngle, std::min(maxPitchAngle, currentPitch));
}

Vector3 CameraManager::SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity, 
                                 float smoothTime, float deltaTime, float maxSpeed)
{
    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    
    Vector3 change = current - target;
    Vector3 originalTo = target;
    
    // Create vector with same direction but clamped magnitude
    float maxChangeMag = maxSpeed * smoothTime;
    float currentMag = change.length();
    Vector3 normalizedChange = change.normalisedCopy();
    change = normalizedChange * std::min(maxChangeMag, currentMag);
    
    Vector3 temp = (currentVelocity + (change * omega)) * deltaTime;
    currentVelocity = (currentVelocity - (temp * omega)) * exp;
    Vector3 output = target + (change + temp) * exp;
    
    // Check if we've overshot the target
    if ((originalTo - current).dotProduct(output - originalTo) > 0) {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }
    
    return output;
}

void CameraManager::updateCameraPosition(SceneNode* playerNode)
{
    if (!playerNode || !camNode) return;

    Vector3 playerPosition = playerNode->getPosition();
    Quaternion playerOrientation = playerNode->getOrientation();

    // Extract only Y rotation (yaw) from player
    Radian yaw;
    Vector3 axis;
    playerOrientation.ToAngleAxis(yaw, axis);
    Quaternion yawOnly(Degree(yaw.valueDegrees()), Vector3::UNIT_Y);

    // Base camera position (behind player)
    Vector3 baseOffset = Vector3(30, 100, 150);
    
    // Apply only Y rotation to camera offset
    Vector3 rotatedOffset = yawOnly * baseOffset;
    
    // Apply pitch rotation (up/down) to camera
    Quaternion pitchRotation(Degree(currentPitch), Vector3::UNIT_X);
    Vector3 finalOffset = pitchRotation * rotatedOffset;
    
    // Calculate final camera position
    Vector3 targetCameraPosition = playerPosition + finalOffset;

    // Smoothly interpolate camera position
    Vector3 smoothedPosition = SmoothDamp(
        camNode->getPosition(),
        targetCameraPosition,
        currentCameraVelocity,
        cameraSmoothTime,
        1.0f/60.0f,
        maxCameraSpeed
    );

    camNode->setPosition(smoothedPosition);
    
    // Make camera look in the same direction as player
    Vector3 lookAtPosition = playerPosition + (yawOnly * Vector3::NEGATIVE_UNIT_Z * 100.0f);
    camNode->lookAt(lookAtPosition, Node::TS_WORLD);
}

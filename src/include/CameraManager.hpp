#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

using namespace Ogre;

class CameraManager {
public:
    CameraManager(SceneManager* scnMgr, RenderWindow* window);
    ~CameraManager();

    void createCamera();
    void updateCameraPosition(SceneNode* playerNode);
    
    // Getters
    Camera* getCamera() const { return cam; }
    SceneNode* getCameraNode() const { return camNode; }
    Viewport* getViewport() const { return vp; }
    
    // Camera control
    void setPitch(float pitch);
    float getPitch() const { return currentPitch; }
    
private:
    // Helper function for smooth camera movement
    Vector3 SmoothDamp(const Vector3& current, const Vector3& target, Vector3& currentVelocity, 
                       float smoothTime, float deltaTime, float maxSpeed);

    SceneManager* scnMgr;
    RenderWindow* window;
    Camera* cam;
    SceneNode* camNode;
    Viewport* vp;
    
    // Camera movement parameters
    Vector3 currentCameraVelocity;
    float currentPitch;
    float maxPitchAngle;
    float cameraSmoothTime;
    float maxCameraSpeed;
    Vector3 cameraOffset;
};

#endif // CAMERA_MANAGER_HPP

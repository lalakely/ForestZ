#include "../include/Plane.hpp"

PlaneZ::PlaneZ() {
    // Constructor implementation (if needed)
}
PlaneZ::~PlaneZ() {
    // Destructor implementation (if needed)
}

void PlaneZ::createPlane(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld) {
  planeCountX = PLANE_X; // Number of planes along the X-axis
  planeCountZ = PLANE_Z; // Number of planes along the Z-axis
  planeSize = PLANE_SIZE; // Size of each plane
   for (int x = 0; x < planeCountX; ++x) {
        for (int z = 0; z < planeCountZ; ++z) {
            // Calculate the position of the current plane
            float posX = (x - planeCountX / 2) * planeSize;
            float posZ = (z - planeCountZ / 2) * planeSize;

            // Create a unique name for the plane
            std::string planeName = "ground_" + std::to_string(x) + "_" + std::to_string(z);

            // Create the plane mesh
            Plane plane(Vector3::UNIT_Y, 0);
            MeshManager::getSingleton().createPlane(
                planeName, RGN_DEFAULT,
                plane,
                planeSize, planeSize, 20, 20,
                true,
                1, 5, 5,
                Vector3::UNIT_Z);

            // Create the plane entity
            Entity* groundEntity = scnMgr->createEntity(planeName);
            SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode(Vector3(posX, 0, posZ));
            groundNode->attachObject(groundEntity);
            groundEntity->setCastShadows(false);

            // Apply the grass material to the plane
            groundEntity->setMaterialName("Examples/Grass");

            // Add physics to the plane
            btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0); // Plane along Y-axis

            // Set the plane's position in the physics world
            btTransform planeTransform;
            planeTransform.setIdentity();
            planeTransform.setOrigin(btVector3(posX, 0, posZ));

            // Create a rigid body for the plane
            btScalar mass = 0.0f; // Static object (mass = 0)
            btVector3 localInertia(0, 0, 0);
            btDefaultMotionState* motionState = new btDefaultMotionState(planeTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, planeShape, localInertia);
            btRigidBody* planeBody = new btRigidBody(rbInfo);
            planeBody->setFriction(1000000000.8f); 

            // Add the rigid body to the physics world
            dynamicsWorld->addRigidBody(planeBody);

        }
    }
}
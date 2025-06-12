#ifndef LIB_HPP
#define LIB_HPP

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h" // Include CameraMan for camera movement
#include "BulletDebugDrawer.hpp" // Include the debug drawer header
#include <btBulletDynamicsCommon.h> // Include Bullet Physics header
#include <cstdlib> // For std::rand and std::srand
#include <ctime>   // For std::time
#include <btBulletDynamicsCommon.h> // Include Bullet Physics headers
#include <iostream>


#define PLANE_X 10
#define PLANE_Z 10
#define PLANE_SIZE 1000.0f // Size of each plane
#define PLANE_WIDTH PLANE_X*PLANE_SIZE // Width of the plane
#define PLANE_HEIGHT PLANE_Z*PLANE_SIZE // Height of the plane

#define TREE_NUMBER 400 // Number of trees to generate
#define DISTANCE_RENDER_TREE 2000.0f // Distance threshold for rendering treess

#define PLAYER_SPEED 400.0f
#define PLAYER_SPRINT_MULTIPLIER 1.5f // Sprint multiplier for running
#define ZOMBIE_SPEED 100.0f // Speed of zombies
#define ZOMBIES_NUMBER 1 // Number of zombies to spawn
#define BULLET_SPEED 2000.0f // Vitesse des balles augmentée pour un meilleur gameplay
#define LEVEL_TRANSITION_TIME 3.0f // Temps de transition entre les niveaux

using namespace Ogre;
using namespace OgreBites;
using namespace Ogre::RTShader;
using namespace std;

#endif
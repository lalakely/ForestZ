#include "../include/BulletDebugDrawer.hpp"
#include <OgreStringConverter.h>
#include <iostream>

BulletDebugDrawer::BulletDebugDrawer(Ogre::SceneManager* sceneMgr)
    : mSceneMgr(sceneMgr), mLines(nullptr), mDebugMode(btIDebugDraw::DBG_DrawWireframe), mDrawing(false)
{
    Ogre::String name = "BulletDebugLines_" + Ogre::StringConverter::toString(rand());
    mLines = mSceneMgr->createManualObject(name);
    mLines->setDynamic(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mLines);
}

BulletDebugDrawer::~BulletDebugDrawer()
{
    if (mLines) {
        mSceneMgr->destroyManualObject(mLines);
    }
}

void BulletDebugDrawer::begin()
{
    if (!mLines) return;
    mLines->clear();
    mLines->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST); // Replace with your debug material
    mDrawing = true;
}

void BulletDebugDrawer::end()
{
    if (!mLines || !mDrawing) return;
    mLines->end();
    mDrawing = false;
}

void BulletDebugDrawer::clear()
{
    if (mLines) mLines->clear();
}

void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    if (!mLines || !mDrawing) return;

    // Validate the positions to avoid extreme values
    const float MAX_VALID_COORD = 1e6f; // Define a maximum valid coordinate range
    if ((from.x() > MAX_VALID_COORD || from.x() < -MAX_VALID_COORD ||
         from.y() > MAX_VALID_COORD || from.y() < -MAX_VALID_COORD ||
         from.z() > MAX_VALID_COORD || from.z() < -MAX_VALID_COORD ||
         to.x() > MAX_VALID_COORD || to.x() < -MAX_VALID_COORD ||
         to.y() > MAX_VALID_COORD || to.y() < -MAX_VALID_COORD ||
         to.z() > MAX_VALID_COORD || to.z() < -MAX_VALID_COORD)) 
    {
        
        return;
    }

    Ogre::ColourValue ogreColor(color.x(), color.y(), color.z());

    mLines->position(Ogre::Vector3(from.x(), from.y(), from.z()));
    mLines->colour(ogreColor);
    mLines->position(Ogre::Vector3(to.x(), to.y(), to.z()));
    mLines->colour(ogreColor);
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
    std::cerr << "[Bullet Debug] " << warningString << std::endl;
}

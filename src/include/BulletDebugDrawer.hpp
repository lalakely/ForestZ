#ifndef BULLET_DEBUG_DRAWER_HPP
#define BULLET_DEBUG_DRAWER_HPP

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

class BulletDebugDrawer : public btIDebugDraw {
public:
    BulletDebugDrawer(Ogre::SceneManager* sceneMgr);
    ~BulletDebugDrawer();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void reportErrorWarning(const char* warningString) override;
    void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
    void draw3dText(const btVector3&, const char*) override {}

    void setDebugMode(int debugMode) override { mDebugMode = debugMode; }
    int getDebugMode() const override { return mDebugMode; }

    void begin();   // Must be called before debug drawing
    void end();     // Must be called after debug drawing
    void clear();   // Optional: clear old lines

private:
    Ogre::SceneManager* mSceneMgr;
    Ogre::ManualObject* mLines;
    int mDebugMode;
    bool mDrawing;
};

#endif // BULLET_DEBUG_DRAWER_HPP

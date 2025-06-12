#ifndef WELCOME_PAGE_HPP
#define WELCOME_PAGE_HPP

#include <Ogre.h>
#include <OgreTrays.h>
#include <OgreInput.h>
#include <OgreApplicationContext.h>

class WelcomePage : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    WelcomePage();
    virtual ~WelcomePage();

    void setup();
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool keyPressed(const OgreBites::KeyboardEvent& evt);

private:
    void createUI();
    void buttonClicked(OgreBites::Button* button);

    OgreBites::TrayManager* mTrayMgr;
    OgreBites::Button* mPlayButton;
    OgreBites::Button* mSettingsButton;
    OgreBites::Button* mMultiplayerButton;
    OgreBites::Button* mQuitButton;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::SceneNode* mCameraNode;
};

#endif // WELCOME_PAGE_HPP 
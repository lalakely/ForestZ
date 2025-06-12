#include "WelcomePage.hpp"
#include <OgreOverlaySystem.h>

WelcomePage::WelcomePage() : OgreBites::ApplicationContext("ForestZ")
{
}

WelcomePage::~WelcomePage()
{
    if (mTrayMgr) delete mTrayMgr;
}

void WelcomePage::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    // Créer la scène
    mSceneMgr = getRoot()->createSceneManager();
    mSceneMgr->addRenderQueueListener(getOverlaySystem());

    // Configurer la caméra
    mCamera = mSceneMgr->createCamera("MainCamera");
    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCameraNode->attachObject(mCamera);
    mCamera->setNearClipDistance(5);

    // Créer la vue
    Ogre::Viewport* vp = getRenderWindow()->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    // Créer le gestionnaire de Tray
    mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // Créer l'interface utilisateur
    createUI();
}

void WelcomePage::createUI()
{
    // Créer les boutons
    mPlayButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "PlayButton", "Jouer", 200);
    mSettingsButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "SettingsButton", "Paramètres", 200);
    mMultiplayerButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "MultiplayerButton", "Multijoueur", 200);
    mQuitButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "QuitButton", "Quitter", 200);

    // Positionner les boutons
    mPlayButton->setPosition(0, -100);
    mSettingsButton->setPosition(0, -50);
    mMultiplayerButton->setPosition(0, 0);
    mQuitButton->setPosition(0, 50);
}

bool WelcomePage::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return OgreBites::ApplicationContext::frameRenderingQueued(evt);
}

bool WelcomePage::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}

void WelcomePage::buttonClicked(OgreBites::Button* button)
{
    if (button == mPlayButton)
    {
        // Lancer le jeu
        // TODO: Implémenter le lancement du jeu
    }
    else if (button == mSettingsButton)
    {
        // Afficher les paramètres
        // TODO: Implémenter l'affichage des paramètres
    }
    else if (button == mMultiplayerButton)
    {
        // Accéder au multijoueur
        // TODO: Implémenter l'accès au multijoueur
    }
    else if (button == mQuitButton)
    {
        // Quitter le jeu
        getRoot()->queueEndRendering();
    }
} 
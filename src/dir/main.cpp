#include "WelcomePage.hpp"
#include <Ogre.h>
#include <OgreApplicationContext.h>

int main(int argc, char *argv[])
{
    try
    {
        WelcomePage app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

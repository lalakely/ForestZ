#ifndef PLANE_HPP
#define PLANE_HPP

#include "lib.hpp"

class PlaneZ {
    public :
        PlaneZ();
        ~PlaneZ();

        SceneManager* scnMgr;
        int planeCountX;
        int planeCountZ;
        float planeSize;
        void createPlane(SceneManager* scnMgr, btDiscreteDynamicsWorld* dynamicsWorld);
};

#endif
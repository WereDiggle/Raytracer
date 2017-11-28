#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"

#include "PhotonMap.hpp"

PhotonMap::PhotonMap(int numPhotons)
    : numPhotons(numPhotons)
{
}

void PhotonMap::emitLight(SceneNode * root)
{
    // Use the fibonacci sphere algorithm
    // https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
    double offset = 2.0/numPhotons;
    double increment = glm::pi<double>() * (3.0 - glm::sqrt(5.0));

    //std::cout << "PHOTON MAP POINTS" << std::endl;

    for (int i=0; i<numPhotons; ++i) {
        double y = ((i * offset) - 1.0) + (offset / 2.0);
        double r = glm::sqrt(1.0 - y*y);

        double phi = i * increment;

        double x = glm::cos(phi) * r;
        double z = glm::sin(phi) * r;

        glm::dvec3 photonPoint = glm::dvec3(x,y,z);

        //std::cout << std::setprecision(4) << x << "," << y << "," << z << "," << std::endl;
    }
}
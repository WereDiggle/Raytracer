#include <iostream>
#include <iomanip>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"

#include "PhotonMap.hpp"

Photon::Photon(const glm::vec3 & postion, const glm::vec3 & incident, double flux)
    : position(position), incident(incident), flux(flux) {}

double Photon::getDistance(const glm::vec3 & point) {
    return glm::distance(position, point);
}
    
PhotonNode::PhotonNode(Photon * splitPhoton, PhotonNode * lesserNode, PhotonNode * greaterNode)
    : splitPhoton(splitPhoton), lesserNode(lesserNode), greaterNode(greaterNode) {}

PhotonNode::PhotonNode(std::vector<Photon>::iterator first, std::vector<Photon>::iterator last, const glm::vec3 & parentMin, const glm::vec3 & parentMax, char dimension)
{
    // base case
    if (first == last) {

        splitPhoton = &(*first);

        min = splitPhoton.position;
        max = splitPhoton.position;

        lesserNode = nullptr;
        greaterNode = nullptr;
    }

    // General case
    else {

        char nextDimension;

        min = parentMin;
        max = parentMax;
        glm::vec3 lesserMax = max;
        glm::vec3 greaterMin = min;

        // Partition a chunk of the array according to the right dimension
        std::vector<Photon>::iterator middle = first + (last - first)/2;
        switch(dimension) {
            case 'x':
                std::nth_element(first, middle, last+1, lessThanX());
                lesserMax.x = (*middle).position.x;
                greaterMin.x = (*middle).position.x;
                nextDimension = 'y';
                break;
            case 'y':
                std::nth_element(first, middle, last+1, lessThanY());
                lesserMax.y = (*middle).position.y;
                greaterMin.y = (*middle).position.y;
                nextDimension = 'z';
                break;
            case 'z':
                std::nth_element(first, middle, last+1, lessThanZ());
                lesserMax.z = (*middle).position.z;
                greaterMin.z = (*middle).position.z;
                nextDimension = 'x';
                break;
        }

        splitPhoton = &(*middle);

        // Make some new nodes
        lesserNode = new PhotonNode(first, middle, nextDimension, min, lesserMax);
        greaterNode = new PhotonNode(middle+1, last, nextDimension, greaterMin, max);
    }
}

// TODO: implement
void PhotonNode::nearestNeighbours(std::vector<Photon*> & nearestPhotons, int k, const glm::vec3 & point, const glm::vec3 & min, const glm::vec3 & max, char dimension) {

    // TODO: need a better way to keep track of which photons to replace



}

// TODO: test
Photon* PhotonNode::nearestNeighbour(const glm::vec3 & point, const glm::vec3 & min, const glm::vec3 & max, char dimension) {

    // leaf node base case
    if (lesserNode == nullptr && greaterNode == nullptr) {
        return splitPhoton;
    }

    // Get photon from corresponding bounding box
    PhotonNode * nextNode = nullptr;
    PhotonNode * otherNode = nullptr;
    char nextDimension; 
    switch(dimension) {
        case 'x':
            if (point.x < splitPhoton.position.x) {
                nextNode = lesserNode;
                otherNode = greaterNode;
            }
            else {
                nextNode = greaterNode;
                otherNode = lesserNode;
            }
            nextDimension = 'y';
            break;
        case 'y':
            if (point.y < splitPhoton.position.y) {
                nextNode = lesserNode;
                otherNode = greaterNode;
            }
            else {
                nextNode = greaterNode;
                otherNode = lesserNode;
            }
            nextDimension = 'z';
            break;
        case 'z':
            if (point.z < splitPhoton.position.z) {
                nextNode = lesserNode;
                otherNode = greaterNode;
            }
            else {
                nextNode = greaterNode;
                otherNode = lesserNode;
            }
            nextDimension = 'x';
            break;
    }

    Photon * nearestPhoton = nextNode->nearestNeighbours(point, nextDimension);
    double nearestDistance = nearestPhoton->getDistance(point);

    // check if other bounding box could be closer
    glm::vec3 closestOtherPoint = glm::vec3(glm::clamp(point.x, otherNode.min.x, otherNode.max.x), 
                                            glm::clamp(point.y, otherNode.min.y, otherNode.max.y), 
                                            glm::clamp(point.z, otherNode.min.z, otherNode.max.z));
    double otherDistance = glm::distance(point, closestOtherPoint);

    if (otherDistance < nearestDistance) {
        Photon * otherPhoton = otherNode->nearestNeighbours(point, nextDimension);
        if (otherPhoton->getDistance(point) < nearestDistance) {
            nearestPhoton = otherPhoton;
        }
    }

    return nearestPhoton;
}

PhotonTree::PhotonTree()
    : root(nullptr), min(glm::vec3(0)), max(glm::vec3(0)) {}

void PhotonTree::buildTree(std::vector<Photon> newPhotons) {
    if (newPhotons.size() == 0) {
        return;
    }
    // TODO: clear out the root node first

    photons = newPhotons;

    root = new PhotonNode(photons.begin(), photons.end()-1, min, max);
}

std::vector<Photon*> PhotonTree::nearestNeighbours(int k, const glm::vec3 & point) {
    std::vector<Photon*> kNearestPhotons;

    root->nearestNeighbours(k, point, min, max); 

    return kNearestPhotons;
}

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
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"

#include "PhotonMap.hpp"

Photon::Photon(const glm::vec3 & postion, const glm::vec3 & incident, const glm::vec3 & flux)
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

        min = splitPhoton->position;
        max = splitPhoton->position;

        lesserNode = nullptr;
        greaterNode = nullptr;
    }

    // General case
    else {

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
                break;
            case 'y':
                std::nth_element(first, middle, last+1, lessThanY());
                lesserMax.y = (*middle).position.y;
                greaterMin.y = (*middle).position.y;
                break;
            case 'z':
                std::nth_element(first, middle, last+1, lessThanZ());
                lesserMax.z = (*middle).position.z;
                greaterMin.z = (*middle).position.z;
                break;
        }

        splitPhoton = &(*middle);

        // Make some new nodes
        lesserNode = new PhotonNode(first, middle, min, lesserMax, nextDimension(dimension));
        greaterNode = new PhotonNode(middle+1, last, greaterMin, max, nextDimension(dimension));
    }
}

// TODO: test 
void PhotonNode::nearestNeighbours(std::vector<Photon*> & nearestPhotons, double range, const glm::vec3 & point, char dimension) {

    // leaf node base case
    if (lesserNode == nullptr && greaterNode == nullptr) {
        if (glm::distance(splitPhoton->position, point) <= range) {
            nearestPhotons.push_back(splitPhoton); 
        }
        return;
    }
    // There's only one path to go
    else if (greaterNode == nullptr) {
        return lesserNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }
    else if (lesserNode == nullptr) {
        return greaterNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }

    // Check lesser bounding box
    glm::vec3 closestLesserPoint = glm::vec3(glm::clamp(point.x, lesserNode->min.x, lesserNode->max.x), 
                                             glm::clamp(point.y, lesserNode->min.y, lesserNode->max.y), 
                                             glm::clamp(point.z, lesserNode->min.z, lesserNode->max.z));

    // This means that a point inside the bounding box COULD be within range. Otherwise, no point is within range
    if (glm::distance(point, closestLesserPoint) <= range) {
        lesserNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }

    // Check greater bounding box
    glm::vec3 closestGreaterPoint = glm::vec3(glm::clamp(point.x, greaterNode->min.x, greaterNode->max.x), 
                                             glm::clamp(point.y, greaterNode->min.y, greaterNode->max.y), 
                                             glm::clamp(point.z, greaterNode->min.z, greaterNode->max.z));

    if (glm::distance(point, closestGreaterPoint) <= range) {
        lesserNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }
}

char PhotonNode::nextDimension(char dimension) {

    char nextDimension; 
    switch(dimension) {
        case 'x':
            nextDimension = 'y';
            break;
        case 'y':
            nextDimension = 'z';
            break;
        case 'z':
            nextDimension = 'x';
            break;
    }

    return nextDimension;
}

// TODO: test
Photon* PhotonNode::nearestNeighbour(const glm::vec3 & point, char dimension) {

    // leaf node base case
    if (lesserNode == nullptr && greaterNode == nullptr) {
        return splitPhoton;
    }
    // There's only one path to go
    else if (greaterNode == nullptr) {
        return lesserNode->nearestNeighbour(point, nextDimension(dimension));
    }
    else if (lesserNode == nullptr) {
        return greaterNode->nearestNeighbour(point, nextDimension(dimension));
    }

    // Get photon from corresponding bounding box
    PhotonNode * nextNode = nullptr;
    PhotonNode * otherNode = nullptr;
    char nextDimension; 
    switch(dimension) {
        case 'x':
            if (point.x < splitPhoton->position.x) {
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
            if (point.y < splitPhoton->position.y) {
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
            if (point.z < splitPhoton->position.z) {
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

    Photon * nearestPhoton = nextNode->nearestNeighbour(point, nextDimension);
    double nearestDistance = nearestPhoton->getDistance(point);

    // check if other bounding box could be closer
    glm::vec3 closestOtherPoint = glm::vec3(glm::clamp(point.x, otherNode->min.x, otherNode->max.x), 
                                            glm::clamp(point.y, otherNode->min.y, otherNode->max.y), 
                                            glm::clamp(point.z, otherNode->min.z, otherNode->max.z));
    double otherDistance = glm::distance(point, closestOtherPoint);

    if (otherDistance < nearestDistance) {
        Photon * otherPhoton = otherNode->nearestNeighbour(point, nextDimension);
        if (otherPhoton->getDistance(point) < nearestDistance) {
            nearestPhoton = otherPhoton;
        }
    }

    return nearestPhoton;
}

PhotonTree::PhotonTree()
    : root(nullptr), min(glm::vec3(0)), max(glm::vec3(0)) {}

void PhotonTree::buildTree(std::vector<Photon> newPhotons, const glm::vec3 & min, const glm::vec3 & max) {
    if (newPhotons.size() == 0) {
        return;
    }
    // TODO: clear out the root node first

    this->min = min;
    this->max = max;
    photons = newPhotons;

    root = new PhotonNode(photons.begin(), photons.end()-1, min, max);
}

std::vector<Photon*> PhotonTree::nearestNeighbours(double range, const glm::vec3 & point) {
    std::vector<Photon*> nearestPhotons;

    root->nearestNeighbours(nearestPhotons, range, point); 

    return nearestPhotons;
}

PhotonMap::PhotonMap(int numPhotons)
    : numPhotons(numPhotons)
{
}


void PhotonMap::emitLight(SceneNode * root, const std::list<Light *> & lights)
{
    if (lights.size() == 0) {
        return;
    }

    std::vector<Photon> allPhotons;

    // Use the fibonacci sphere algorithm
    // https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
    double offset = 2.0/numPhotons;
    double increment = glm::pi<double>() * (3.0 - glm::sqrt(5.0));

    //std::cout << "PHOTON MAP POINTS" << std::endl;

    // We're dividing the number of photons over the number of lights in the scene
    for (int i=0; i < numPhotons/lights.size() ; ++i) {
        double y = ((i * offset) - 1.0) + (offset / 2.0);
        double r = glm::sqrt(1.0 - y*y);

        double phi = i * increment;

        double x = glm::cos(phi) * r;
        double z = glm::sin(phi) * r;

        glm::vec3 photonDirection = glm::vec3(x,y,z);


        for (Light * light : lights) {
            Ray photonRay = Ray(light->position, light->position + photonDirection);

            root->castRay(photonRay);

            // TODO: loop until we hit a diffuse surface. Use monte carlo.
            // We do not handle flux changing frequencies passing through coloured surfaces
            // ie. white light going through green glass
            Intersection photonIntersection;

            // flux of the Photon dependent on light intensity;
            // For now just use hard coded factors
            if (photonIntersection.isHit) {
                Photon newPhoton = Photon(photonIntersection.pointHit, photonRay->direction, light->intensity);
                allPhotons.push_back(newPhoton);
            }
        }
    }

    // We have a vector of photons now, make the Photon tree
}
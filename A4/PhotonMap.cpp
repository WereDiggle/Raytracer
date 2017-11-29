#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"
#include "Intersect.hpp"

#include "PhotonMap.hpp"

Photon::Photon(const glm::vec3 & position, const glm::vec3 & incident, const glm::vec3 & flux)
    : position(position), incident(incident), flux(flux) 
{
}

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
        //std::cout << "PhotonNode nearestNeighbours leaf node base case" << std::endl;
        if (glm::distance(splitPhoton->position, point) <= range) {
            nearestPhotons.push_back(splitPhoton); 
        }
        return;
    }
    // There's only one path to go
    else if (greaterNode == nullptr) {
        //std::cout << "PhotonNode nearestNeighbours greaterNode is nullptr" << std::endl;
        return lesserNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }
    else if (lesserNode == nullptr) {
        //std::cout << "PhotonNode nearestNeighbours lesserNode is nullptr" << std::endl;
        return greaterNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }


    // Check lesser bounding box
    glm::vec3 closestLesserPoint = glm::vec3(glm::clamp(point.x, lesserNode->min.x, lesserNode->max.x), 
                                             glm::clamp(point.y, lesserNode->min.y, lesserNode->max.y), 
                                             glm::clamp(point.z, lesserNode->min.z, lesserNode->max.z));

    // Check greater bounding box
    glm::vec3 closestGreaterPoint = glm::vec3(glm::clamp(point.x, greaterNode->min.x, greaterNode->max.x), 
                                              glm::clamp(point.y, greaterNode->min.y, greaterNode->max.y), 
                                              glm::clamp(point.z, greaterNode->min.z, greaterNode->max.z));

    //std::cout << "PhotonNode nearestNeighbours general case: range: " << range << std::endl;


    // This means that a point inside the bounding box COULD be within range. Otherwise, no point is within range
    if (glm::distance(point, closestLesserPoint) <= range) {
        lesserNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
    }

    if (glm::distance(point, closestGreaterPoint) <= range) {
        greaterNode->nearestNeighbours(nearestPhotons, range, point, nextDimension(dimension));
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

    //std::cout << "photon tree bounding box: min: " << glm::to_string(min) << ", max: " << glm::to_string(max) << std::endl;

    root = new PhotonNode(photons.begin(), photons.end()-1, min, max);
}

std::vector<Photon*> PhotonTree::nearestNeighbours(double range, const glm::vec3 & point) {
    std::vector<Photon*> nearestPhotons;

    root->nearestNeighbours(nearestPhotons, range, point); 
    //std::cout << "Num Photons around point: " << nearestPhotons.size() << std::endl; 

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

    // For tracking progress
    int start_time = clock();

    std::vector<Photon> allPhotons;

    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());

    // Use the fibonacci sphere algorithm
    // https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
    double offset = 2.0/numPhotons;
    double increment = glm::pi<double>() * (3.0 - glm::sqrt(5.0));

    srand(2017);
    std::cout << "EMITTING PHOTONS" << std::endl;

    // We're dividing the number of photons over the number of lights in the scene
    double progress = 0.0;
    for (int i=0; i < numPhotons/lights.size() ; ++i) {
        double y = ((i * offset) - 1.0) + (offset / 2.0);
        double r = glm::sqrt(1.0 - y*y);

        double phi = i * increment;

        double x = glm::cos(phi) * r;
        double z = glm::sin(phi) * r;

        glm::vec3 photonDirection = glm::vec3(x,y,z);

        int lightIndex = 0;
        for (Light * light : lights) {
            Ray photonRay = Ray(light->position, light->position + photonDirection);

            Intersect photonIntersect = root->castRay(photonRay);

            // TODO: loop until we hit a diffuse surface. Use monte carlo.
            // We do not handle flux changing frequencies passing through coloured surfaces
            // ie. white light going through green glass

            // reflectiveness, diffuse, and trasparency are probabilities that sum to 1.0
            // We'll just default to diffuse

            for (int numRecursions=0; photonIntersect.isHit && numRecursions<Intersect::MAX_DEPTH; ++numRecursions) {

                double prob = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

                if (photonIntersect.reflectiveness > 0.0 && prob <= photonIntersect.reflectiveness) {
                    photonIntersect = root->castRay(photonIntersect.getReflectionRay());
                }
                else if (photonIntersect.transparency > 0.0 && prob <= photonIntersect.reflectiveness + photonIntersect.transparency) {
                    photonIntersect = root->castRay(photonIntersect.getRefractionRay());
                }
                else {
                    break;
                }
            }

            // flux of the Photon dependent on light intensity;
            // For now just use hard coded factors
            if (photonIntersect.isHit) {
                //std::cout << "photonIntersect pointHit: " << glm::to_string(photonIntersect.pointHit) << std::endl;
                Photon newPhoton = Photon(photonIntersect.pointHit, photonRay.direction, light->colour);
                //std::cout << "newPhoton position: " << glm::to_string(newPhoton.position) << std::endl;
                allPhotons.push_back(newPhoton);

                min.x = glm::min(newPhoton.position.x, min.x);
                min.y = glm::min(newPhoton.position.y, min.y);
                min.z = glm::min(newPhoton.position.z, min.z);

                max.x = glm::max(newPhoton.position.x, max.x);
                max.y = glm::max(newPhoton.position.y, max.y);
                max.z = glm::max(newPhoton.position.z, max.z);

                //std::cout << "calculating photon bounding box: min: " << glm::to_string(min) << ", max: " << glm::to_string(max) << std::endl;
            }

			// Progress calculations
			int intermediate_time = clock();
			std::cout << std::setfill('0') << std::setw(5) << std::fixed << std::setprecision(2);
			std::cout << "Progress: " << (progress/numPhotons) * 100 << "%";
			double elapsedSeconds = (intermediate_time - start_time)/double(CLOCKS_PER_SEC);
			int minutes = (int)elapsedSeconds / 60;
			double seconds = elapsedSeconds - minutes*60;
			std::cout << " [Elapsed Time: " << minutes << " minutes, " << seconds << " seconds] \t" << '\r';
			std::cout.flush();

            progress += 1.0;
        }
    }

	std::cout << std::endl;
	int stop_time = clock();
	std::cout << "photon emission took: " << (stop_time - start_time)/double(CLOCKS_PER_SEC) << " seconds" << std::endl;

    // We have a vector of photons now, make the Photon tree
    std::cout << "BUILDING PHOTON TREE" << std::endl;
    photonTree.buildTree(allPhotons, min, max);
    int tree_time = clock();
    std::cout << "PHOTON TREE build time: " << (tree_time - stop_time)/double(CLOCKS_PER_SEC) << " seconds" << std::endl;
}

std::vector<Photon*> PhotonMap::getPhotonsAroundPoint(double range, const glm::vec3 & point) {
    return photonTree.nearestNeighbours(range, point);
}

glm::vec3 PhotonMap::getFluxAroundPoint(double range, const glm::vec3 & point) {
    std::vector<Photon*> nearestPhotons = getPhotonsAroundPoint(range, point);
    glm::vec3 totalFlux = glm::vec3(0);
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        totalFlux += (*it)->flux;
    }

    return totalFlux;
}
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SceneNode.hpp"
#include "Intersect.hpp"
#include "Material.hpp"
#include "Settings.hpp"

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

void PhotonNode::nearestNeighbours(std::vector<Photon*> & nearestPhotons, int k, double range, const glm::vec3 & point, char dimension) {


    // leaf node base case
    if (lesserNode == nullptr && greaterNode == nullptr) {

        if (glm::distance(splitPhoton->position, point) <= range) {

            // There's room in the vector, easy
            if (nearestPhotons.size() < k) {
                nearestPhotons.push_back(splitPhoton); 
            }
            // Gonna have to remove someone
            else if (glm::distance(point, splitPhoton->position) < glm::distance(point, nearestPhotons[k-1]->position)) {
                nearestPhotons[k-1] = splitPhoton;

                // make sure that the last element is the largest
                std::nth_element(nearestPhotons.begin(), nearestPhotons.begin()+k-1, nearestPhotons.end(), lessThanDistance(point));
            }
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

    double lesserDistance = glm::distance(point, closestLesserPoint);

    // This means that a point inside the bounding box COULD be within range. Otherwise, no point is within range
    if (lesserDistance <= range && (nearestPhotons.size() < k || lesserDistance < glm::distance(point, nearestPhotons[k-1]->position))) {
        lesserNode->nearestNeighbours(nearestPhotons, k, range, point, nextDimension(dimension));
    }

    // Check greater bounding box
    glm::vec3 closestGreaterPoint = glm::vec3(glm::clamp(point.x, greaterNode->min.x, greaterNode->max.x), 
                                              glm::clamp(point.y, greaterNode->min.y, greaterNode->max.y), 
                                              glm::clamp(point.z, greaterNode->min.z, greaterNode->max.z));

    double greaterDistance = glm::distance(point, closestGreaterPoint);

    if (greaterDistance <= range && (nearestPhotons.size() < k || greaterDistance < glm::distance(point, nearestPhotons[k-1]->position))) {
        greaterNode->nearestNeighbours(nearestPhotons, k, range, point, nextDimension(dimension));
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

    if (range > 0.0) {
        root->nearestNeighbours(nearestPhotons, range, point); 
    }

    return nearestPhotons;
}

std::vector<Photon*> PhotonTree::nearestNeighbours(int k, double range, const glm::vec3 & point) {
    std::vector<Photon*> nearestPhotons;

    if (k > 0 && range > 0.0) {

        // TODO: actually fix this
        // It doesn't actually get the nearest photons
        //root->nearestNeighbours(nearestPhotons, k, range, point); 

        root->nearestNeighbours(nearestPhotons, range, point);

        // Only want the k closest points
        if (nearestPhotons.size() > k) {
            std::nth_element(nearestPhotons.begin(), nearestPhotons.begin()+k-1, nearestPhotons.end(), lessThanDistance(point));
            nearestPhotons.erase(nearestPhotons.begin()+k, nearestPhotons.end());
        }
    }

    return nearestPhotons;
}

int PhotonTree::size() {
    return photons.size();
}

PhotonMap::PhotonMap(int numPhotons)
    : numPhotons(numPhotons)
{
}


void PhotonMap::emitLight(SceneNode * root, const std::list<Light *> & lights)
{
    if (lights.size() == 0 || numPhotons <= 0) {
        return;
    }

    // For tracking progress
    int start_time = clock();

    std::vector<Photon> allPhotons;
    std::vector<Photon> causticPhotons;

    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());

    glm::vec3 causticMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 causticMax = glm::vec3(std::numeric_limits<float>::min());

    // Use the fibonacci sphere algorithm
    // https://stackoverflow.com/questions/9600801/evenly-distributing-n-points-on-a-sphere
    double offset = 2.0/numPhotons;
    double increment = glm::pi<double>() * (3.0 - glm::sqrt(5.0));

    srand(2017);
    std::cout << "EMITTING PHOTONS" << std::endl;

    double progress = 0.0;

    // We're dividing the number of photons over the number of lights in the scene
    // TODO: figure out how to split up the power of the lights
    double totalLightPower = 0;
    for (Light * light : lights) {
        totalLightPower += light->colour.r + light->colour.g + light->colour.b;
    }

    for (Light * light : lights) {

        double photonPortion = (light->colour.r + light->colour.g + light->colour.b)/totalLightPower; 
        int photonsForLight = (int) (numPhotons * photonPortion);
        glm::vec3 fluxPerPhoton = PHOTON_LIGHT_SCALE * light->colour / photonsForLight;

        // We want to split up the power from the lights by emitting more photons for brighter lights, so we have photons closer in flux
        for (int i=0; i < photonsForLight; ++i) {

            double y = ((i * offset) - 1.0) + (offset / 2.0);
            double r = glm::sqrt(1.0 - y*y);

            double phi = i * increment;

            double x = glm::cos(phi) * r;
            double z = glm::sin(phi) * r;

            glm::vec3 photonDirection = glm::vec3(x,y,z);

            int lightIndex = 0;

            Ray photonRay = Ray(light->position, light->position + photonDirection);

            Intersect photonIntersect = root->castRay(photonRay);

            // TODO: loop until we hit a diffuse surface. Use russian roulette.
            // We do not handle flux changing frequencies passing through coloured surfaces
            // ie. white light going through green glass
            // Also we don't handle diffuse light reflections yet

            // reflectiveness, diffuse, and trasparency are probabilities that sum to 1.0
            // We'll just default to diffuse

            bool isCaustic = false;
            for (int numRecursions=0; photonIntersect.isHit && numRecursions<MAX_DEPTH; ++numRecursions) {

                double prob = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                glm::vec3 diffuseColour = photonIntersect.getColour();

                double diffuseReflectiveness;

                // We want all photons to bounce at least once
                if (numRecursions < MIN_PHOTON_BOUNCE) {
                    diffuseReflectiveness = photonIntersect.diffuse;
                }
                else {
                    diffuseReflectiveness = 0.5 * photonIntersect.diffuse * glm::clamp(diffuseColour.r + diffuseColour.g + diffuseColour.b, 0.0f, 3.0f)/3.0f;
                }

                if (photonIntersect.reflectiveness > 0.0 && prob <= photonIntersect.reflectiveness) {
                    photonIntersect = root->castRay(photonIntersect.getReflectionRay());
                    isCaustic = true;
                }
                else if (photonIntersect.transparency > 0.0 && prob <= photonIntersect.reflectiveness + photonIntersect.transparency) {
                    photonIntersect = root->castRay(photonIntersect.getRefractionRay());
                    isCaustic = true;
                }
                else if (diffuseReflectiveness > 0.0 && prob <= photonIntersect.reflectiveness + photonIntersect.transparency + diffuseReflectiveness) {
                    // TODO: diffuse reflection
                    // Random direction for this photon that faces the same was as the normal, bump maps might do some weird stuff with this 
                    //photonIntersect = root->castRay(photonIntersect.getDiffuseReflectionRay());
                    break;
                    isCaustic = false;
                }
                else {
                    if (numRecursions < MIN_PHOTON_BOUNCE) {
                        std::cout << "This should not happen" << std::endl;
                    }
                    break;
                }
            }

            // flux of the Photon dependent on light intensity;
            // For now just use hard coded factors
            if (photonIntersect.isHit) {
                //std::cout << "photonIntersect pointHit: " << glm::to_string(photonIntersect.pointHit) << std::endl;
                Photon newPhoton = Photon(photonIntersect.pointHit, -photonRay.direction, fluxPerPhoton);
                //std::cout << "newPhoton position: " << glm::to_string(newPhoton.position) << std::endl;
                if (isCaustic) {
                    causticPhotons.push_back(newPhoton);

                    causticMin.x = glm::min(newPhoton.position.x, causticMin.x);
                    causticMin.y = glm::min(newPhoton.position.y, causticMin.y);
                    causticMin.z = glm::min(newPhoton.position.z, causticMin.z);

                    causticMax.x = glm::max(newPhoton.position.x, causticMax.x);
                    causticMax.y = glm::max(newPhoton.position.y, causticMax.y);
                    causticMax.z = glm::max(newPhoton.position.z, causticMax.z);
                }
                else {
                    allPhotons.push_back(newPhoton);

                    min.x = glm::min(newPhoton.position.x, min.x);
                    min.y = glm::min(newPhoton.position.y, min.y);
                    min.z = glm::min(newPhoton.position.z, min.z);

                    max.x = glm::max(newPhoton.position.x, max.x);
                    max.y = glm::max(newPhoton.position.y, max.y);
                    max.z = glm::max(newPhoton.position.z, max.z);
                }
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
    causticPhotonTree.buildTree(causticPhotons, causticMin, causticMax);
    int tree_time = clock();
    std::cout << "PHOTON TREE build time: " << (tree_time - stop_time)/double(CLOCKS_PER_SEC) << " seconds" << std::endl;
}

std::vector<Photon*> PhotonMap::getPhotonsAroundPoint(double range, const glm::vec3 & point) {
    if (photonTree.size() <= 0) {
        return std::vector<Photon*>();
    }
    return photonTree.nearestNeighbours(range, point);
}

std::vector<Photon*> PhotonMap::getCausticPhotonsAroundPoint(double range, const glm::vec3 & point) {
    if (causticPhotonTree.size() <= 0) {
        return std::vector<Photon*>();
    }
    return causticPhotonTree.nearestNeighbours(range, point);
}

std::vector<Photon*> PhotonMap::getPhotonsAroundPoint(int k, double range, const glm::vec3 & point) {
    if (photonTree.size() <= 0) {
        return std::vector<Photon*>();
    }
    return photonTree.nearestNeighbours(k, range, point);
}

std::vector<Photon*> PhotonMap::getCausticPhotonsAroundPoint(int k, double range, const glm::vec3 & point) {
    if (causticPhotonTree.size() <= 0) {
        return std::vector<Photon*>();
    }
    return causticPhotonTree.nearestNeighbours(k, range, point);
}

glm::vec3 PhotonMap::getFluxAroundPoint(double range, const glm::vec3 & point) {
    if (photonTree.size() <= 0) {
        return glm::vec3(0);
    }

    std::vector<Photon*> nearestPhotons = getPhotonsAroundPoint(range, point);
    glm::vec3 totalFlux = glm::vec3(0);
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        totalFlux += (*it)->flux;
    }

    return totalFlux;
}

glm::vec3 PhotonMap::getCausticFluxAroundPoint(double range, const glm::vec3 & point) {
    if (causticPhotonTree.size() <= 0) {
        return glm::vec3(0);
    }
    
    std::vector<Photon*> nearestPhotons = getCausticPhotonsAroundPoint(range, point);
    glm::vec3 totalFlux = glm::vec3(0);
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        totalFlux += (*it)->flux;
    }

    return totalFlux;
}

glm::vec3 PhotonMap::getFluxAroundPoint(int k, double range, const glm::vec3 & point) {
    if (photonTree.size() <= 0) {
        return glm::vec3(0);
    }
    
    std::vector<Photon*> nearestPhotons = getPhotonsAroundPoint(k, range, point);
    glm::vec3 totalFlux = glm::vec3(0);
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        totalFlux += (*it)->flux;
    }

    return totalFlux;
}

glm::vec3 PhotonMap::getCausticFluxAroundPoint(int k, double range, const glm::vec3 & point) {
    if (causticPhotonTree.size() <= 0) {
        return glm::vec3(0);
    }
    
    std::vector<Photon*> nearestPhotons = getCausticPhotonsAroundPoint(k, range, point);
    glm::vec3 totalFlux = glm::vec3(0);
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        totalFlux += (*it)->flux;
    }

    return totalFlux;
}

glm::vec3 PhotonMap::getIrradiance(int k, double range, const glm::vec3 & point, const glm::vec3 & viewDirection, const glm::vec3 & surfaceNormal, Material * material) {
    if (photonTree.size() <= 0) {
        return glm::vec3(0);
    }
    std::vector<Photon*> nearestPhotons = getPhotonsAroundPoint(k, range, point);

    if (nearestPhotons.size() <= 0) {
        return glm::vec3(0);
    }

    // Surface area
    std::nth_element(nearestPhotons.begin(), nearestPhotons.end()-1, nearestPhotons.end(), lessThanDistance(point));
    double radius = glm::clamp(glm::distance(point, (*(nearestPhotons.end()-1))->position), 0.1f, (float) PHOTON_GATHER_RANGE);
    double circleArea = glm::pi<double>() * radius * radius;

    // Bidirectional reflectance distribution and photon flux
    glm::vec3 sumIrradiance;
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        // TODO: we're just multiplying by the flux of the photon for now. 
        // Not sure if I should take the distance of the photon from the point or the incident into account
        double diffuseBRDF = glm::clamp(glm::dot(surfaceNormal, (*it)->incident), 0.0f, 1.0f);

        sumIrradiance += diffuseBRDF * (*it)->flux;
    }

    glm::vec3 avgIrradiance = sumIrradiance/circleArea;
    //if (nearestPhotons.size() < PHOTON_GATHER_NUM) {
    //    std::cout << "dark spot" << std::endl;
    //    std::cout << "sum Irradiance " << glm::to_string(sumIrradiance) << std::endl; 
    //    std::cout << "circleArea " << circleArea << std::endl;
    //    std::cout << "numPhotons " << nearestPhotons.size() << std::endl;
    //}

    return avgIrradiance;

}

glm::vec3 PhotonMap::getCausticIrradiance(int k, double range, const glm::vec3 & point, const glm::vec3 & viewDirection, const glm::vec3 & surfaceNormal, Material * material) {
    if (causticPhotonTree.size() <= 0) {
        return glm::vec3(0);
    }
    std::vector<Photon*> nearestPhotons = getCausticPhotonsAroundPoint(k, range, point);

    if (nearestPhotons.size() <= 0) {
        return glm::vec3(0);
    }

    // Surface area
    std::nth_element(nearestPhotons.begin(), nearestPhotons.end()-1, nearestPhotons.end(), lessThanDistance(point));
    double radius = glm::clamp(glm::distance(point, (*(nearestPhotons.end()-1))->position), 0.1f, (float) PHOTON_GATHER_RANGE);
    double circleArea = glm::pi<double>() * radius * radius;

    // Bidirectional reflectance distribution and photon flux
    glm::vec3 sumIrradiance;
    for (std::vector<Photon*>::iterator it = nearestPhotons.begin(); it != nearestPhotons.end(); ++it) {
        // TODO: we're just multiplying by the flux of the photon for now. 
        // Not sure if I should take the distance of the photon from the point or the incident into account
        double diffuseBRDF = glm::clamp(glm::dot(surfaceNormal, (*it)->incident), 0.0f, 1.0f);

        double distanceWeight = ((radius - glm::distance(point, (*it)->position))/radius) * CAUSTIC_SHARPNESS;

        sumIrradiance += diffuseBRDF * (*it)->flux * distanceWeight;
    }

    glm::vec3 avgIrradiance = sumIrradiance/circleArea;

    return avgIrradiance;

}

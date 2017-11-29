#pragma once

#include <glm/glm.hpp>
#include <vector>

class SceneNode;

// Stores info about a single photon interaction with a surface
class Photon {

public:

    // position of the photon
    glm::vec3 position;

    // direction that it came from
    glm::vec3 incident;

    // how powerful the photon is
    double flux;

    Photon(const glm::vec3 & postion, const glm::vec3 & incident, double flux);

    double getDistance(const glm::vec3 & point);
};

// Compare object for x 
struct lessThanX {
    bool operator() (const Photon & a, const Photon & b) const {
        return a.position.x < b.position.x;
    }
};

// Compare object for y 
struct lessThanY {
    bool operator() (const Photon & a, const Photon & b) const {
        return a.position.y < b.position.y;
    }
};

// Compare object for z 
struct lessThanZ {
    bool operator() (const Photon & a, const Photon & b) const {
        return a.position.z < b.position.z;
    }
};

class PhotonNode {

public:

    glm::vec3 min;
    glm::vec3 max;

    PhotonNode * lesserNode;
    PhotonNode * greaterNode;
    Photon * splitPhoton;

    PhotonNode(Photon * splitPhoton, PhotonNode * lesserNode = nullptr, PhotonNode * greaterNode = nullptr);
    PhotonNode(std::vector<Photon>::iterator first, std::vector<Photon>::iterator last, const glm::vec3 & parentMin, const glm::vec3 & parentMax, char dimension = 'x');

    std::vector<Photon*> nearestNeighbours(int k, const glm::vec3 & point);

    Photon* nearestNeighbours(const glm::vec3 & point);

    // TODO: implement
    //~PhotonNode();

};

// It's actually just a 3-dimensional kd-tree for Photons
class PhotonTree {

    // Bounding box
    // TODO: we can get the bounding info when we process the raycast intersections from photon emission
    //       just need to pass that info here, or maybe store in the photon map;
    glm::vec3 min;
    glm::vec3 max;

    // The root node
    PhotonNode * root;

    // Where we actually store the photons that the nodes point to
    std::vector<Photon> photons;

public:

    PhotonTree();

    void buildTree(std::vector<Photon> newPhotons);

    // TODO: implement
    //~PhotonTree();

    std::vector<Photon*> nearestNeighbours(int k, const glm::vec3 & point);

};

// Handles all emitting, storing, and gathering of photon info
class PhotonMap {

    int numPhotons;

    PhotonTree photonTree;

public:

    PhotonMap(int numPhotons);

    void emitLight(SceneNode * root);

};
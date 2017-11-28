#pragma once

class SceneNode;

// Stores info about a single photon interaction with a surface
class Photon {

};

// It's actually just a 3-dimensional kd-tree for Photons
class PhotonTree {

};

// Handles all emitting, storing, and gathering of photon info
class PhotonMap {

    int numPhotons;

public:

    PhotonMap(int numPhotons);

    void emitLight(SceneNode * root);

};
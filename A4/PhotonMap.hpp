#pragma once

class SceneNode;

class PhotonMap {

    int numPhotons;

public:

    PhotonMap(int numPhotons);

    void emitLight(SceneNode * root);

};
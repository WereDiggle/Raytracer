#pragma once

#include <list>
#include <glm/glm.hpp>

#include "Ray.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Light.hpp"

class SceneNode;
class PhotonMap;

// This class handles the information received when a Ray hits something
class Intersect {

    bool getRefractionRay(Ray & refractedRay);

public:

    static PhotonMap * globalPhotonMap;

    Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit);

    // used for when the intersection doesn't hit anything
    Intersect();

    glm::vec3 getColour();

    Ray getReflectionRay();

    Ray getRefractionRay();

    Ray getDiffuseReflectionRay();

    glm::vec3 getLighting(const glm::vec3 & ambient, const std::list<Light *> & lights, SceneNode * root, int depth = 0, double contribution = 1.0);

    Intersect transformIntersect(const glm::mat4 & m);
    
    bool isHit;

    Ray ray;

    double distanceHit;

    Material *material;
    
    glm::vec3 normalHit;

    glm::vec3 pointHit;

    double textureU;
    double textureV;

    Texture* bitmap;
    Texture* bumpmap;

    double reflectiveness;
    double diffuse;
    double transparency;
    double refraction;
};
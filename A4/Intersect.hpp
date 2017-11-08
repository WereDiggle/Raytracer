#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"
#include "Material.hpp"
#include "Light.hpp"

// This class handles the information received when a Ray hits something
class Intersect {

public:
    Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit);

    // used for when the intersection doesn't hit anything
    Intersect();

    glm::vec3 getLighting(Light * light);

    Intersect transformIntersect(const glm::mat4 & m);
    
    bool isHit;

    Ray ray;

    double distanceHit;

    Material *material;
    
    glm::vec3 normalHit;

    glm::vec3 pointHit;
};
#pragma once

#include <glm/glm.hpp>

#include "Ray.hpp"

// This class handles the information received when a Ray hits something
class Intersect {

public:
    Intersect(const Ray & ray, const glm::vec3 & colour, bool isHit, double distanceHit);

    // used for when the intersection doesn't hit anything
    Intersect();
    
    glm::vec3 colour;
    bool isHit;
    Ray ray;
    double distanceHit;
};
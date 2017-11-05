#include "Intersect.hpp"

Intersect::Intersect(const Ray & ray, const glm::vec3 & colour, bool isHit, double distanceHit)
    : ray(ray), colour(colour), isHit(isHit), distanceHit(distanceHit) {}

Intersect::Intersect()
    : ray(ray), colour(glm::vec3()), isHit(false), distanceHit(-1.0) {}
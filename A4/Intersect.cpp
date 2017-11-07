#include "Intersect.hpp"

Intersect::Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit)
    : ray(ray), material(nullptr), isHit(isHit), distanceHit(distanceHit), normalHit(normalHit) {}

Intersect::Intersect()
    : ray(ray), material(nullptr), isHit(false), distanceHit(-1.0), normalHit(glm::vec3(0)) {}

glm::vec3 Intersect::getLighting(Light * light) {
    if (material == nullptr || !isHit) {
        return glm::vec3(0);
    }

    // surfaceNormal, lightDirection, lightIntensity, lightDistance, lightFalloff, viewDirection
    glm::vec3 pointHit = ray.pointAtDistance(distanceHit);
    glm::vec3 pointToLight = light->position - pointHit;
    return material->getLighting(normalHit, glm::normalize(pointToLight), light->colour, glm::length(pointToLight), light->falloff, -ray.direction);
}
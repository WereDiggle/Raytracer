#include <iostream>
#include <glm/ext.hpp>

#include "Intersect.hpp"

Intersect::Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit)
    : ray(ray), material(nullptr), isHit(isHit), distanceHit(distanceHit), normalHit(normalHit) {
        if (isHit) {
            pointHit = ray.pointAtDistance(distanceHit);
        }
    }

Intersect::Intersect()
    : ray(ray), material(nullptr), isHit(false), distanceHit(-1.0), normalHit(glm::vec3(0)), pointHit(glm::vec3(0)) {}

glm::vec3 Intersect::getLighting(Light * light) {
    if (material == nullptr || !isHit) {
        return glm::vec3(0);
    }

    // surfaceNormal, lightDirection, lightIntensity, lightDistance, lightFalloff, viewDirection
    glm::vec3 pointToLight = light->position - pointHit;
    return material->getLighting(glm::normalize(normalHit), glm::normalize(pointToLight), light->colour, glm::length(pointToLight), light->falloff, -ray.direction);
}

Intersect Intersect::transformIntersect(const glm::mat4 & m) {

    Intersect newIntersect = Intersect();
    newIntersect.ray = ray.transformRay(m);
    newIntersect.isHit = isHit;
    newIntersect.pointHit = glm::vec3(m * glm::vec4(pointHit, 1));
    newIntersect.distanceHit = glm::length(newIntersect.ray.origin - newIntersect.pointHit);
    newIntersect.normalHit = glm::normalize(glm::mat3(glm::transpose(glm::inverse(m))) * normalHit);
    newIntersect.material = material;

    return newIntersect;
}
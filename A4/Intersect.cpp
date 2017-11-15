#include <iostream>
#include <glm/ext.hpp>

#include "SceneNode.hpp"

#include "Intersect.hpp"

Intersect::Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit)
    : ray(ray), material(nullptr), isHit(isHit), distanceHit(distanceHit), normalHit(normalHit), textureU(0), textureV(0) {
        if (isHit) {
            pointHit = ray.pointAtDistance(distanceHit);
        }
    }

Intersect::Intersect()
    : ray(ray), material(nullptr), isHit(false), distanceHit(-1.0), normalHit(glm::vec3(0)), pointHit(glm::vec3(0)), textureU(0), textureV(0) {}

glm::vec3 Intersect::getLighting(const std::list<Light *> & lights, SceneNode * root) {
    if (material == nullptr || !isHit) {
        return glm::vec3(0);
    }

    // Add each individual light contribution
    glm::vec3 totalLighting = glm::vec3(0);
    for (Light * light : lights) {
        Ray shadowRay = Ray(pointHit, light->position);
        Intersect shadowIntersect = root->castShadowRay(shadowRay);
        if (!shadowIntersect.isHit) {
            glm::vec3 pointToLight = light->position - pointHit;
            // surfaceNormal, lightDirection, lightIntensity, lightDistance, lightFalloff, viewDirection
            totalLighting += material->getLighting(glm::normalize(normalHit), glm::normalize(pointToLight), light->colour, glm::length(pointToLight), light->falloff, -ray.direction, textureU, textureV);
        }
    }

    return totalLighting;


}

Intersect Intersect::transformIntersect(const glm::mat4 & m) {

    Intersect newIntersect = Intersect();
    newIntersect.ray = ray.transformRay(m);
    newIntersect.isHit = isHit;
    newIntersect.pointHit = glm::vec3(m * glm::vec4(pointHit, 1));
    newIntersect.distanceHit = glm::length(newIntersect.ray.origin - newIntersect.pointHit);
    newIntersect.normalHit = glm::mat3(glm::transpose(glm::inverse(m))) * normalHit;
    newIntersect.material = material;
    newIntersect.textureU = textureU;
    newIntersect.textureV = textureV;

    return newIntersect;
}
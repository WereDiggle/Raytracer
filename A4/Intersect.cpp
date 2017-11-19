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

glm::vec3 Intersect::getLighting(const glm::vec3 & ambient, const std::list<Light *> & lights, SceneNode * root) {
    if (material == nullptr || !isHit) {
        return glm::vec3(0);
    }

    // TODO: not entirely sure about this for ambient light
    glm::vec3 matColour = material->getColour();
    glm::vec3 totalLighting = glm::vec3(ambient.r * matColour.r, 
                                        ambient.g * matColour.g, 
                                        ambient.b * matColour.b);

    // Add each individual light contribution
    for (Light * light : lights) {
        Ray shadowRay = Ray(pointHit, light->position);
        Intersect shadowIntersect = root->castRay(shadowRay);
        // Apply lighting if the shadow ray doesn't hit anything or the closest thing the shadow ray hits 
        glm::vec3 pointToLight = light->position - pointHit;
        double lightDistance = glm::length(pointToLight);
        if (!shadowIntersect.isHit || shadowIntersect.distanceHit >= lightDistance) {
            // surfaceNormal, lightDirection, lightIntensity, lightDistance, lightFalloff, viewDirection, u, v, bitmap, and bumpmap textures
            totalLighting += material->getLighting(glm::normalize(normalHit), 
                                                   glm::normalize(pointToLight), light->colour, lightDistance, light->falloff, 
                                                   -ray.direction, 
                                                   textureU, textureV, bitmap, bumpmap);
        }
    }

    glm::vec3 texColour = bitmap->getColour(textureU, textureV);
    glm::vec3 totalColour = glm::vec3(texColour.r * totalLighting.r,
                                      texColour.g * totalLighting.g,
                                      texColour.b * totalLighting.b);

    return totalColour;
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
    newIntersect.bitmap = bitmap;
    newIntersect.bumpmap = bumpmap;

    return newIntersect;
}
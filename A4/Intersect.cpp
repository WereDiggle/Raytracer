#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtx/component_wise.hpp>

#include "SceneNode.hpp"

#include "Intersect.hpp"

const int Intersect::MAX_DEPTH = 10;

const double Intersect::MIN_CONTRIBUTION = 0.01;

Intersect::Intersect(const Ray & ray, bool isHit, double distanceHit, const glm::vec3 & normalHit)
    : ray(ray), material(nullptr), isHit(isHit), distanceHit(distanceHit), normalHit(normalHit), textureU(0), textureV(0) {
        if (isHit) {
            pointHit = ray.pointAtDistance(distanceHit);
        }
    }

Intersect::Intersect()
    : ray(ray), material(nullptr), isHit(false), distanceHit(-1.0), normalHit(glm::vec3(0)), pointHit(glm::vec3(0)), textureU(0), textureV(0) {}

Ray Intersect::getReflectionRay() {
    glm::vec3 reflection = (ray.direction - 2 * glm::dot(ray.direction, normalHit) * normalHit);
    //std::cout << "incident ray: " << glm::to_string(ray.direction) << ", normal: " << glm::to_string(normalHit) << ", reflection: " << glm::to_string(reflection) << std::endl;
    return Ray(pointHit, pointHit + (ray.direction - 2 * glm::dot(ray.direction, normalHit) * normalHit));
}

bool Intersect::getRefractionRay(Ray & refractedRay) {
    // ray direction and normal are both unit vectors, so the dot product should just be the cos of the angle between them
    double incidentCos = glm::clamp(glm::dot(ray.direction, glm::normalize(normalHit)), -1.0f, 1.0f);
    double refractionIndex = refraction;
    glm::vec3 refractionNorm = normalHit;

    // We're going into the surface from vacuum
    if (incidentCos < 0) {
        incidentCos = -incidentCos;
        refractionIndex = 1.0/refractionIndex;
    }
    // We're exiting the surface into vacuum
    else {
        refractionNorm = -refractionNorm; 
    }

    double refractedFactor = 1.0 - refractionIndex * refractionIndex * (1.0 - incidentCos * incidentCos);

    glm::vec3 refractedDirection;
    if (refractedFactor < 0) {
        //std::cout << "TOTAL Internal refraction" << std::endl;
        // Total internal refraction, use the reflection direction
        // don't shoot two rays in the same direction, just use the reflection case
        return false;
    }
    else {
        refractedDirection = refractionIndex * ray.direction + (refractionIndex * incidentCos - glm::sqrt(refractedFactor)) * refractionNorm;
        //std::cout << "refraction ray from: " << glm::to_string(pointHit) << " to " << glm::to_string(pointHit + refractedDirection) << std::endl;
        refractedRay = Ray(pointHit, pointHit + refractedDirection);
        return true;
    }
}

static glm::vec3 blend(const glm::vec3 & a, const glm::vec3 b) {
    return glm::vec3(a.r * b.r, a.g * b.g, a.b * b.b);
}

glm::vec3 Intersect::getLighting(const glm::vec3 & ambient, const std::list<Light *> & lights, SceneNode * root, int depth, double contribution) {
    if (material == nullptr || !isHit) {
        return glm::vec3(0);
    }

    //std::cout << std::endl << "ray depth: " << depth << std::endl;

    glm::vec3 totalLighting = glm::vec3(0);

    double transparencyPortion = transparency;
    double reflectivenessPortion = reflectiveness;
    double diffusePortion = diffuse;

    // online handle reflection if we haven't reached the end of ray casting depth yet
    if (transparencyPortion > 0 && depth < MAX_DEPTH && contribution > MIN_CONTRIBUTION) {
        Ray refractionRay;
        if (getRefractionRay(refractionRay)) {
            // Refraction happened
            //std::cout << "original ray direction: " << glm::to_string(ray.direction) << std::endl;
            //std::cout << "refracted ray direction: " << glm::to_string(refractionRay.direction) << std::endl;
            Intersect refractionIntersect = root->castRay(refractionRay);
            //std::cout << "refracted ray hits: " << glm::to_string(refractionIntersect.pointHit) << std::endl;
            totalLighting += transparencyPortion * refractionIntersect.getLighting(ambient, lights, root, depth + 1, contribution * transparencyPortion);
        }
        else {
            // Nope, actually reflection happened
            reflectivenessPortion += transparencyPortion;
        }
    }
    if (reflectivenessPortion > 0 && depth < MAX_DEPTH && contribution > MIN_CONTRIBUTION) {
        //std::cout << "reflection ray" << std::endl;
        Ray reflectionRay = getReflectionRay();
        // root->castRay will not recurse, that all has to happen here, in getLighting
        Intersect reflectionIntersect = root->castRay(reflectionRay);
        totalLighting += reflectivenessPortion * reflectionIntersect.getLighting(ambient, lights, root, depth + 1, contribution * reflectivenessPortion);
    }
    if (diffusePortion > 0) {

        //std::cout << "ray hit a diffuse surface at: " << glm::to_string(pointHit) << std::endl;
        glm::vec3 matColour = material->getColour();
        glm::vec3 totalDiffuseLighting = blend(ambient, matColour);

        // Add each individual light contribution
        for (Light * light : lights) {

            Ray shadowRay = Ray(pointHit, light->position);
            Intersect shadowIntersect = root->castRay(shadowRay);
            glm::vec3 pointToLight = light->position - pointHit;
            double lightDistance = glm::length(pointToLight);

            // Apply lighting if the shadow ray doesn't hit anything or the closest thing the shadow ray hits 
            // TODO: turn shadows back on, also make them work better with transparent objects
            //if (!shadowIntersect.isHit || shadowIntersect.distanceHit >= lightDistance) {
                // surfaceNormal, lightDirection, lightIntensity, lightDistance, lightFalloff, viewDirection, u, v, bitmap, and bumpmap textures
                totalDiffuseLighting += material->getLighting(glm::normalize(normalHit), 
                                                              glm::normalize(pointToLight), light->colour, lightDistance, light->falloff, 
                                                              -ray.direction, 
                                                              textureU, textureV, bitmap, bumpmap);
            //}
        }

        totalLighting += diffusePortion * totalDiffuseLighting;
    }

    return totalLighting;
}

Intersect Intersect::transformIntersect(const glm::mat4 & m) {

    Intersect newIntersect = Intersect();
    newIntersect.ray = ray.transformRay(m);
    newIntersect.isHit = isHit;
    newIntersect.pointHit = glm::vec3(m * glm::vec4(pointHit, 1));
    newIntersect.distanceHit = glm::length(newIntersect.ray.origin - newIntersect.pointHit);
    newIntersect.normalHit = glm::normalize(glm::mat3(glm::transpose(glm::inverse(m))) * normalHit);
    newIntersect.material = material;
    newIntersect.textureU = textureU;
    newIntersect.textureV = textureV;
    newIntersect.bitmap = bitmap;
    newIntersect.bumpmap = bumpmap;
    newIntersect.reflectiveness = reflectiveness;
    newIntersect.diffuse = diffuse;
    newIntersect.transparency = transparency;
    newIntersect.refraction = refraction;

    return newIntersect;
}
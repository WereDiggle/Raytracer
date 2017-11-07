#pragma once

#include <glm/glm.hpp>

class Ray {
public:
    Ray(const glm::vec3 & origin, const glm::vec3 & point);

    glm::vec3 pointAtDistance(double distance) const;
    glm::vec3 pointAtDistance(float distance) const;

    glm::vec3 direction;
    glm::vec3 origin;
    glm::vec3 point;
    //double maxDistance;
    double minDistance = 0.01;
};
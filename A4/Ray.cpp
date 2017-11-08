#include "Ray.hpp"

Ray::Ray(const glm::vec3 & origin, const glm::vec3 & point)
    : origin(origin), point(point) {
        direction = glm::normalize(point - origin);
    }

glm::vec3 Ray::pointAtDistance(float distance) const {
    return origin + distance*direction;
}

glm::vec3 Ray::pointAtDistance(double distance) const {
    return origin + (float)distance * direction;
}

Ray Ray::transformRay(const glm::mat4 & m) {
    return Ray(glm::vec3(m * glm::vec4(origin, 1)), glm::vec3(m * glm::vec4(point, 1)));
}
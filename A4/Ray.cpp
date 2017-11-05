#include "Ray.hpp"

Ray::Ray(const glm::vec3 & origin, const glm::vec3 & point)
    : origin(origin), point(point) {
        direction = glm::normalize(point - origin);
    }
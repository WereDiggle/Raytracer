#include "Primitive.hpp"
#include "polyroots.hpp"

#include <iostream>

/*
Primitive
#########
*/

Primitive::~Primitive()
{
}

Intersect Primitive::checkIntersection(const Ray & ray) { 
    return Intersect();
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

/*
Non-heirarchical Sphere
#######################
*/

NonhierSphere::~NonhierSphere()
{
}

Intersect NonhierSphere::checkIntersection(const Ray & ray) {
    //std::cout << "non hier sphere checkIntersection" << std::endl;
    glm::vec3 newOrigin = ray.origin - m_pos;
    double a = 1.0;
    double b = 2 * (glm::dot(ray.direction, newOrigin));
    double c = glm::dot(newOrigin, newOrigin) - m_radius*m_radius;
    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots) > 0;

    // Take the lowest non-negative root
    if (numRoots == 2) {
        double distanceHit = (roots[0] > 0 && roots[0] < roots[1]) ? roots[0] : roots[1];
        return Intersect(ray, glm::vec3(0), distanceHit > 0, distanceHit);
    }
    else if (numRoots == 1) {
        return Intersect(ray, glm::vec3(0), roots[0] > 0, roots[0]);
    }
    else {
        return Intersect(ray, glm::vec3(0), false, -1.0);
    }
}

NonhierBox::~NonhierBox()
{
}

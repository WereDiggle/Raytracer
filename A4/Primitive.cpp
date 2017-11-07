#include "Primitive.hpp"
#include "polyroots.hpp"

#include <iostream>
#include <glm/ext.hpp>

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
        return Intersect(ray, distanceHit > 0, distanceHit, glm::normalize(ray.pointAtDistance(distanceHit) - m_pos));
    }
    else if (numRoots == 1) {
        return Intersect(ray, roots[0] > 0, roots[0], glm::normalize(ray.pointAtDistance(roots[0]) - m_pos));
    }
    else {
        return Intersect();
    }
}

NonhierBox::~NonhierBox()
{
}

Intersect NonhierBox::checkIntersection(const Ray & ray) {
    // TODO: six planes constructed from m_pos and m_size
    // TODO: move these to static const
    // normals are just 6
    // Even indices are positive faces
    glm::vec3 normals[6] = {
        // x axis
        glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),

        // y axis
        glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),

        // z axis
        glm::vec3(0, 0, 1), glm::vec3(0, 0, -1),
    };

    // index 0 for positive normals, index 1 for negative normals
    glm::vec3 points[2] = {
        m_pos+m_size*glm::vec3(1), m_pos,
    };

    
    //glm::vec3 bounds[6] = {

    // Find all planes that the ray intersects with
    // It's at most 3
    //std::cout << "non hier box check intersection " << std::endl;
    //std::cout << "ray direction: " << glm::to_string(ray.direction) << std::endl;
    int planesHit = 0;
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6 && planesHit < 3; i++) {
        // Intersects plane if dot product of direction and normal is negative
        // t is positive along the ray
        // TODO: store and reuse these dot products for effieciency
        if (glm::dot(ray.direction, normals[i]) < 0 && glm::dot((ray.origin - points[i%2]), normals[i]) > 0) {

            double distanceHit = (glm::dot((ray.origin - points[i%2]), normals[i]))/(-glm::dot(ray.direction, normals[i]));
            
            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            // for each axis that we need to check
            bool withinBounds = true;
            if (withinBounds && normals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= m_pos.x && pointHit.x <= m_pos.x + m_size;
            }
            if (withinBounds && normals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= m_pos.y && pointHit.y <= m_pos.y + m_size;
            }
            if (withinBounds && normals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= m_pos.z && pointHit.z <= m_pos.z + m_size;
            }

            //std::cout << "ray intersected with plane " << glm::to_string(normals[i]) << " at " << glm::to_string(pointHit) << " " << withinBounds << std::endl;
            planesHit++;

            // It's a hit!
            if (withinBounds) {
                // since we're using normals, the ray should only hit one face
                closestIntersect = Intersect(ray, true, distanceHit, normals[i]);
                break;
            }

        }
    }
    //std::cout << std::endl;

    return closestIntersect;
}
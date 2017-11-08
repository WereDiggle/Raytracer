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
    size_t numRoots = quadraticRoots(a, b, c, roots);

    // Take the lowest non-negative root
    if (numRoots == 2) {
        double distanceHit = (roots[0] > 0 && roots[0] < roots[1]) ? roots[0] : roots[1];
        return Intersect(ray, distanceHit > ray.minDistance, distanceHit, glm::normalize(ray.pointAtDistance(distanceHit) - m_pos));
    }
    else if (numRoots == 1) {
        return Intersect(ray, roots[0] > ray.minDistance, roots[0], glm::normalize(ray.pointAtDistance(roots[0]) - m_pos));
    }
    else {
        return Intersect();
    }
}

const glm::dvec3 NonhierBox::normals[6] = {
        // x axis
        glm::dvec3(1, 0, 0), glm::dvec3(-1, 0, 0),

        // y axis
        glm::dvec3(0, 1, 0), glm::dvec3(0, -1, 0),

        // z axis
        glm::dvec3(0, 0, 1), glm::dvec3(0, 0, -1),
    };


NonhierBox::~NonhierBox()
{
}

Intersect NonhierBox::checkIntersection(const Ray & ray) {
    // normals are just 6
    // Even indices are positive faces
    // index 0 for positive normals, index 1 for negative normals

    // Find all planes that the ray intersects with
    // It's at most 3
    //std::cout << "non hier box check intersection " << std::endl;
    //std::cout << "ray direction: " << glm::to_string(ray.direction) << std::endl;
    //std::cout << glm::to_string(normals[0]) << std::endl;

    //int planesHit = 0;
    //Intersect closestIntersect = Intersect();
    //for (int i=0; i<6 && planesHit < 3; i++) {
    //    // Intersects plane if dot product of direction and normal is negative
    //    // t is positive along the ray
    //    // float directNormDot, pointNormDot;
    //    if ( /*(directNormDot =*/ glm::dot(glm::dvec3(ray.direction), normals[i])/*)*/ < 0 && /*(pointNormDot =*/ glm::dot((glm::dvec3(ray.origin) - points[i%2]), normals[i])/*)*/ > 0) {

    //        //double distanceHit = pointNormDot/(-directNormDot);
    //        double distanceHit = glm::dot((glm::dvec3(ray.origin) - points[i%2]), normals[i])/(-glm::dot(glm::dvec3(ray.direction), normals[i]));
    //        
    //        // Intersects face if intersect point within bounds
    //        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

    //        // for each axis that we need to check
    //        bool withinBounds = true;

    //        std::cout << "distanceHit: " << distanceHit << std::endl;
    //        std::cout << "dimension: " << glm::to_string(m_dimension) << std::endl;
    //        std::cout << "m_pos: " << glm::to_string(m_pos) << std::endl;
    //        if (withinBounds && normals[i].x == 0) {
    //            withinBounds = withinBounds && pointHit.x >= m_pos.x && pointHit.x <= m_pos.x + m_dimension.x;
    //        }
    //        if (withinBounds && normals[i].y == 0) {
    //            withinBounds = withinBounds && pointHit.y >= m_pos.y && pointHit.y <= m_pos.y + m_dimension.y;
    //        }
    //        if (withinBounds && normals[i].z == 0) {
    //            withinBounds = withinBounds && pointHit.z >= m_pos.z && pointHit.z <= m_pos.y + m_dimension.z;
    //        }

    //        //std::cout << "ray intersected with plane " << glm::to_string(normals[i]) << " at " << glm::to_string(pointHit) << " " << withinBounds << std::endl;
    //        planesHit++;

    //        // It's a hit!
    //        if (withinBounds) {
    //            // since we're using normals, the ray should only hit one face
    //            closestIntersect = Intersect(ray, true, distanceHit, normals[i]);
    //            break;
    //        }

    //    }
    //}
    
    //std::cout << std::endl;

    int planesHit = 0;
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6 && planesHit < 3; i++) {
        // Intersects plane if dot product of direction and normal is negative
        // t is positive along the ray
        // TODO: store and reuse these dot products for effieciency
        std::cout << "ray direction: " << glm::to_string(glm::dvec3(ray.direction)) << " " << glm::to_string(ray.direction) << std::endl;
        std::cout << "ray direction: " << glm::to_string(glm::dvec3(ray.origin)) << " " << glm::to_string(ray.origin) << std::endl;
        if (glm::dot(glm::dvec3(ray.direction), normals[i]) < 0 && glm::dot((glm::dvec3(ray.origin) - points[i%2]), normals[i]) > 0) {

            double distanceHit = (glm::dot((glm::dvec3(ray.origin) - points[i%2]), normals[i]))/(-glm::dot(glm::dvec3(ray.direction), normals[i]));
            
            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            // for each axis that we need to check
            bool withinBounds = true;
            if (withinBounds && normals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= m_pos.x && pointHit.x <= m_pos.x + m_dimension.x;
            }
            if (withinBounds && normals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= m_pos.y && pointHit.y <= m_pos.y + m_dimension.y;
            }
            if (withinBounds && normals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= m_pos.z && pointHit.z <= m_pos.z + m_dimension.z;
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
    

    return closestIntersect;
}
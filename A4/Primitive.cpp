#include "Primitive.hpp"
#include "polyroots.hpp"


/*
Primitive
#########
*/

const glm::vec3 Primitive::axisNormals[6] = {
        // x axis
        glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),

        // y axis
        glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),

        // z axis
        glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)
    };

Primitive::~Primitive()
{
}

Intersect Primitive::checkIntersection(const Ray & ray) { 
    return Intersect();
}

Intersect Primitive::checkDoubleSidedIntersection(const Ray & ray) { 
    return Intersect();
}

glm::vec3 Primitive::getUpV(double u, double v) {
    return glm::vec3(0, 1, 0);
}

/*
Sphere
#######################
*/

Sphere::~Sphere()
{
}

// Just nonhier sphere with m_pos = (0,0,0) and m_radius = 1
Intersect Sphere::checkIntersection(const Ray & ray) {
    //std::cout << "non hier sphere checkIntersection" << std::endl;
    double a = 1.0;
    double b = 2 * (glm::dot(ray.direction, ray.origin));
    double c = glm::dot(ray.origin, ray.origin) - 1.0;
    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots);

    double distanceHit = ray.minDistance;
    // Take the lowest non-negative root
    if (numRoots == 2) {
        distanceHit = glm::min(roots[0], roots[1]);
        if (distanceHit <= ray.minDistance) {
            distanceHit = glm::max(roots[0], roots[1]);
        }
    }
    else if (numRoots == 1) {
        distanceHit = roots[0];
    }

    if (distanceHit > ray.minDistance) {
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);
        glm::vec3 normalHit = glm::normalize(pointHit);

        // Need to check the ray and normal are facing opposite directions 
        // In the case of the perpendicular, it's not a hit
        if (glm::dot(ray.direction, normalHit) < 0)  {

            double azimuth = glm::atan(-pointHit.z, pointHit.x);
            double elevation = glm::asin(glm::clamp(pointHit.y, -1.0f, 1.0f));

            double u = (azimuth/2.0)/glm::pi<double>() + 0.5;
            double v = elevation/glm::pi<double>() + 0.5;

            Intersect retIntersect = Intersect(ray, true, distanceHit, normalHit);
            retIntersect.textureU = u;
            retIntersect.textureV = v;

            return retIntersect;
        }
    }

    return Intersect();
}

Intersect Sphere::checkDoubleSidedIntersection(const Ray & ray) {

    //std::cout << "Sphere checkDoubleSidedIntersection" << std::endl;

    double a = 1.0;
    double b = 2 * (glm::dot(ray.direction, ray.origin));
    double c = glm::dot(ray.origin, ray.origin) - 1.0;
    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots);

    double distanceHit = ray.minDistance;
    // Take the lowest non-negative root
    if (numRoots == 2) {
        distanceHit = glm::min(roots[0], roots[1]);
        if (distanceHit <= ray.minDistance) {
            distanceHit = glm::max(roots[0], roots[1]);
        }
    }
    else if (numRoots == 1) {
        distanceHit = roots[0];
    }

    //std::cout << "roots[0]: " << roots[0] << ", roots[1]: " << roots[1] << std::endl;
    //std::cout << "Sphere checkDoubleSidedIntersection distanceHit: " << distanceHit << std::endl;
    if (distanceHit > ray.minDistance) {
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);
        glm::vec3 normalHit = glm::normalize(pointHit);

        // In the case of the perpendicular, it's not a hit
        if (glm::dot(ray.direction, normalHit) != 0)  {
            //std::cout << "Sphere checkDoubleSidedIntersection hit" << std::endl;

            double azimuth = glm::atan(-pointHit.z, pointHit.x);
            double elevation = glm::asin(glm::clamp(pointHit.y, -1.0f, 1.0f));

            double u = (azimuth/2.0)/glm::pi<double>() + 0.5;
            double v = elevation/glm::pi<double>() + 0.5;

            Intersect retIntersect = Intersect(ray, true, distanceHit, normalHit);
            retIntersect.textureU = u;
            retIntersect.textureV = v;

            return retIntersect;
        }
    }

    return Intersect();
}

/*
Plane
#######################

a square plane from (0,0,0) to (1,1,0)
With a normal pointing in (0,0,1)
So it would be visible when looking down the z axis

*/

Plane::~Plane()
{
}

Intersect Plane::checkIntersection(const Ray & ray) {

    Intersect closestIntersect = Intersect();

    // Intersects plane if dot product of direction and normal is negative
    // distanceHit is positive along the ray
    double dirNormDot, originPointNormDot;
    if ((dirNormDot = glm::dot(ray.direction, axisNormals[4])) < 0 && (originPointNormDot = glm::dot(ray.origin, axisNormals[4])) > 0) {

        double distanceHit = originPointNormDot/-dirNormDot;
        
        // Intersects face if intersect point within bounds
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

        // for each axis that we need to check
        bool withinBounds = pointHit.x >= 0 && pointHit.x <= 1.0 && pointHit.y >= 0 && pointHit.y <= 1.0;

        // It's a hit!
        if (withinBounds) {
            closestIntersect = Intersect(ray, distanceHit > ray.minDistance, distanceHit, axisNormals[4]);
            closestIntersect.textureU = pointHit.x;
            closestIntersect.textureV = pointHit.y;
        }
    }
    
    return closestIntersect;
}

Intersect Plane::checkDoubleSidedIntersection(const Ray & ray) {

    Intersect closestIntersect = Intersect();

    // distanceHit is positive along the ray
    double dirNormDot = glm::dot(ray.direction, axisNormals[4]);
    double originPointNormDot = glm::dot(ray.origin, axisNormals[4]);
    double distanceHit = originPointNormDot/-dirNormDot;
    if (distanceHit > ray.minDistance) {
        
        // Intersects face if intersect point within bounds
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

        bool withinBounds = pointHit.x >= 0 && pointHit.x <= 1.0 && pointHit.y >= 0 && pointHit.y <= 1.0;

        // It's a hit!
        if (withinBounds) {
            closestIntersect = Intersect(ray, true, distanceHit, axisNormals[4]);
            closestIntersect.textureU = pointHit.x;
            closestIntersect.textureV = pointHit.y;
        }
    }
    
    return closestIntersect;
}


/*
Cube
#######################
*/

Cube::~Cube()
{
}

Intersect Cube::checkIntersection(const Ray & ray) {

    // Find all planes that the ray intersects with
    // It's at most 3
    int planesHit = 0;
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6 && planesHit < 3; i++) {

        // Intersects plane if dot product of direction and normal is negative
        // distanceHit is positive along the ray
        double dirNormDot, originPointNormDot;
        if ((dirNormDot = glm::dot(ray.direction, axisNormals[i])) < 0 && (originPointNormDot = glm::dot((ray.origin - points[i%2]), axisNormals[i])) > 0) {

            double distanceHit = originPointNormDot/-dirNormDot;
            
            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            double u, v = 0;

            // for each axis that we need to check
            // Two of these will be true
            bool withinBounds = true;
            if (withinBounds && axisNormals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= 0 && pointHit.x <= 1.0;
                u = pointHit.z;
                v = pointHit.x;
            }
            if (withinBounds && axisNormals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= 0 && pointHit.y <= 1.0;
                u = pointHit.x;
                v = pointHit.y;
            }
            if (withinBounds && axisNormals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= 0 && pointHit.z <= 1.0;
                u = pointHit.z;
            }

            planesHit++;

            // It's a hit!
            if (withinBounds) {
                // since we're using normals, the ray should only hit one face
                closestIntersect = Intersect(ray, distanceHit > ray.minDistance, distanceHit, axisNormals[i]);
                closestIntersect.textureU = u;
                closestIntersect.textureV = v;
                break;
            }
        }
    }
    return closestIntersect;
}

Intersect Cube::checkDoubleSidedIntersection(const Ray & ray) {

    // Find all planes that the ray intersects with
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6; i++) {

        // distanceHit is positive along the ray
        double dirNormDot = glm::dot(ray.direction, axisNormals[i]);
        double originPointNormDot = glm::dot((ray.origin - points[i%2]), axisNormals[i]);
        double distanceHit = originPointNormDot/-dirNormDot;
        if (distanceHit > ray.minDistance) {

            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            double u, v = 0;

            // for each axis that we need to check
            // Two of these will be true
            bool withinBounds = true;
            if (withinBounds && axisNormals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= 0 && pointHit.x <= 1.0;
                u = pointHit.z;
                v = pointHit.x;
            }
            if (withinBounds && axisNormals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= 0 && pointHit.y <= 1.0;
                u = pointHit.x;
                v = pointHit.y;
            }
            if (withinBounds && axisNormals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= 0 && pointHit.z <= 1.0;
                u = pointHit.z;
            }

            // It's a hit!
            if (withinBounds) {
                // since we're using normals, the ray should only hit one face
                closestIntersect = Intersect(ray, true, distanceHit, axisNormals[i]);
                closestIntersect.textureU = u;
                closestIntersect.textureV = v;
                break;
            }
        }
    }
    return closestIntersect;
}

/*
Non-heirarchical Sphere
#######################
*/

NonhierSphere::~NonhierSphere()
{
}

Intersect NonhierSphere::checkIntersection(const Ray & ray) {
    glm::vec3 newOrigin = ray.origin - m_pos;
    double a = 1.0;
    double b = 2 * (glm::dot(ray.direction, newOrigin));
    double c = glm::dot(newOrigin, newOrigin) - m_radius*m_radius;
    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots);

    double distanceHit = 0;
    // Take the lowest non-negative root
    if (numRoots == 2) {
        distanceHit = (roots[0] > 0 && roots[0] < roots[1]) ? roots[0] : roots[1];
    }
    else if (numRoots == 1) {
        distanceHit = roots[0];
    }

    if (distanceHit > ray.minDistance) {
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);
        glm::vec3 normalHit = glm::normalize(ray.pointAtDistance(distanceHit) - m_pos);

        if (glm::dot(ray.direction, normalHit) < 0) {

            // Don't need to divide by m_radius for azimuth since atan will just divide that out anyways
            double azimuth = glm::atan(-(pointHit.z - m_pos.z), pointHit.x - m_pos.x);
            double elevation = glm::asin(glm::clamp((pointHit.y - m_pos.y)/m_radius, -1.0, 1.0));

            double u = (azimuth/2.0)/glm::pi<double>() + 0.5;
            double v = elevation/glm::pi<double>() + 0.5;


            Intersect retIntersect = Intersect(ray, true, distanceHit, normalHit);
            retIntersect.textureU = u;
            retIntersect.textureV = v;

            return retIntersect;
        }
    }

    return Intersect();
}

Intersect NonhierSphere::checkDoubleSidedIntersection(const Ray & ray) {
    glm::vec3 newOrigin = ray.origin - m_pos;
    double a = 1.0;
    double b = 2 * (glm::dot(ray.direction, newOrigin));
    double c = glm::dot(newOrigin, newOrigin) - m_radius*m_radius;
    double roots[2];
    size_t numRoots = quadraticRoots(a, b, c, roots);

    double distanceHit = 0;
    // Take the lowest non-negative root
    if (numRoots == 2) {
        distanceHit = (roots[0] > 0 && roots[0] < roots[1]) ? roots[0] : roots[1];
    }
    else if (numRoots == 1) {
        distanceHit = roots[0];
    }

    if (distanceHit > ray.minDistance) {
        glm::vec3 pointHit = ray.pointAtDistance(distanceHit);
        glm::vec3 normalHit = glm::normalize(ray.pointAtDistance(distanceHit) - m_pos);

        if (glm::dot(ray.direction, normalHit) != 0) {

            // Don't need to divide by m_radius for azimuth since atan will just divide that out anyways
            double azimuth = glm::atan(-(pointHit.z - m_pos.z), pointHit.x - m_pos.x);
            double elevation = glm::asin(glm::clamp((pointHit.y - m_pos.y)/m_radius, -1.0, 1.0));

            double u = (azimuth/2.0)/glm::pi<double>() + 0.5;
            double v = elevation/glm::pi<double>() + 0.5;


            Intersect retIntersect = Intersect(ray, true, distanceHit, normalHit);
            retIntersect.textureU = u;
            retIntersect.textureV = v;

            return retIntersect;
        }
    }

    return Intersect();
}

/*
Non-heirarchical Sphere
#######################
*/

NonhierBox::~NonhierBox()
{
}

Intersect NonhierBox::checkIntersection(const Ray & ray) {

    // Find all planes that the ray intersects with
    // It's at most 3
    int planesHit = 0;
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6 && planesHit < 3; i++) {

        // Intersects plane if dot product of direction and normal is negative
        // t is positive along the ray
        double dirNormDot, originPointNormDot;
        if ((dirNormDot = glm::dot(ray.direction, axisNormals[i])) < 0 && (originPointNormDot = glm::dot((ray.origin - points[i%2]), axisNormals[i])) > 0) {

            double distanceHit = originPointNormDot/-dirNormDot;
            
            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            double u, v = 0;
            double xTexture = (pointHit.x - m_pos.x)/m_dimensions.x;
            double yTexture = (pointHit.y - m_pos.y)/m_dimensions.y;
            double zTexture = (pointHit.z - m_pos.z)/m_dimensions.z;

            // for each axis that we need to check
            bool withinBounds = true;
            if (withinBounds && axisNormals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= m_pos.x && pointHit.x <= m_pos.x + m_dimensions.x;
                u = zTexture;
                v = xTexture;
            }
            if (withinBounds && axisNormals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= m_pos.y && pointHit.y <= m_pos.y + m_dimensions.y;
                u = xTexture;
                v = yTexture;
            }
            if (withinBounds && axisNormals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= m_pos.z && pointHit.z <= m_pos.z + m_dimensions.z;
                u = zTexture;
            }

            planesHit++;

            // It's a hit!
            if (withinBounds) {
                // since we're using axisNormals, the ray should only hit one face
                closestIntersect = Intersect(ray, distanceHit > ray.minDistance, distanceHit, axisNormals[i]);
                closestIntersect.textureU = u;
                closestIntersect.textureV = v;
                break;
            }

        }
    
    }
    return closestIntersect;
}

Intersect NonhierBox::checkDoubleSidedIntersection(const Ray & ray) {

    // Find all planes that the ray intersects with
    Intersect closestIntersect = Intersect();
    for (int i=0; i<6; i++) {

        // Intersects plane if dot product of direction and normal is negative
        // t is positive along the ray
        double dirNormDot = glm::dot(ray.direction, axisNormals[i]);
        double originPointNormDot = glm::dot((ray.origin - points[i%2]), axisNormals[i]);
        double distanceHit = originPointNormDot/-dirNormDot;
        if (distanceHit > ray.minDistance) {
            
            // Intersects face if intersect point within bounds
            glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

            double u, v = 0;
            double xTexture = (pointHit.x - m_pos.x)/m_dimensions.x;
            double yTexture = (pointHit.y - m_pos.y)/m_dimensions.y;
            double zTexture = (pointHit.z - m_pos.z)/m_dimensions.z;

            // for each axis that we need to check
            bool withinBounds = true;
            if (withinBounds && axisNormals[i].x == 0) {
                withinBounds = withinBounds && pointHit.x >= m_pos.x && pointHit.x <= m_pos.x + m_dimensions.x;
                u = zTexture;
                v = xTexture;
            }
            if (withinBounds && axisNormals[i].y == 0) {
                withinBounds = withinBounds && pointHit.y >= m_pos.y && pointHit.y <= m_pos.y + m_dimensions.y;
                u = xTexture;
                v = yTexture;
            }
            if (withinBounds && axisNormals[i].z == 0) {
                withinBounds = withinBounds && pointHit.z >= m_pos.z && pointHit.z <= m_pos.z + m_dimensions.z;
                u = zTexture;
            }

            // It's a hit!
            if (withinBounds) {
                // since we're using axisNormals, the ray should only hit one face
                closestIntersect = Intersect(ray, distanceHit > ray.minDistance, distanceHit, axisNormals[i]);
                closestIntersect.textureU = u;
                closestIntersect.textureV = v;
                break;
            }
        }
    }

    return closestIntersect;
}
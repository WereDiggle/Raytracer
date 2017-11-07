#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::cout << "Creating a new mesh: " << fname << std::endl;
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );

	if (ifs.fail()) {
		std::cerr << "Could not open: " << fname << std::endl;
		std::cerr << "Error: " << strerror(errno)  << std::endl;
	}

	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		} }
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

Intersect Mesh::checkIntersectionTriangle(const Ray & ray, const Triangle & triangle) {
	// Calculate the face normal of the triangle
	glm::vec3 v1 = m_vertices[triangle.v1];
	glm::vec3 v2 = m_vertices[triangle.v2];
	glm::vec3 v3 = m_vertices[triangle.v3];

	glm::vec3 A = v2 - v1;
	glm::vec3 B = v3 - v1;

	glm::vec3 triangleNormal = glm::cross(A, B);
	//std::cout << "triangle normal: " << glm::to_string(triangleNormal) << std::endl;

	// Calculate the intersection of the normal plane and the ray. Look in the box primitive code for examples. 
	float directionNormalDot, originNormalDot;
	Intersect retIntersect = Intersect();
	if ((directionNormalDot = glm::dot(ray.direction, triangleNormal)) < 0 
	    && (originNormalDot = glm::dot((ray.origin - v1), triangleNormal)) > 0) {

		double distanceHit = originNormalDot/(-directionNormalDot);

		// Calculate Point of intersection if there is one.
		glm::vec3 pointHit = ray.pointAtDistance(distanceHit);

		//std::cout << "hit a triangle plane at " << glm::to_string(pointHit) << std::endl;

		// Check if Point is inside of triangle edges
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 edge2 = v3 - v2;
		glm::vec3 edge3 = v1 - v3;

		glm::vec3 pointVector1 = pointHit - v1;
		glm::vec3 pointVector2 = pointHit - v2;
		glm::vec3 pointVector3 = pointHit - v3;

		bool withinBounds = (glm::dot(triangleNormal, glm::cross(edge1, pointVector1)) >= 0
						  && glm::dot(triangleNormal, glm::cross(edge2, pointVector2)) >= 0
						  && glm::dot(triangleNormal, glm::cross(edge3, pointVector3)) >= 0);

		retIntersect = Intersect(ray, glm::vec3(0), withinBounds, distanceHit);
	}

	return retIntersect;
}

Intersect Mesh::checkIntersection(const Ray & ray) {

	// Iterate over all the triangles to find the closest one hit
	Intersect closestIntersect = Intersect(ray, glm::vec3(0), false, std::numeric_limits<double>::max());
	for (std::vector<Triangle>::iterator it = m_faces.begin(); it != m_faces.end(); ++it) {
		Intersect curIntersect = checkIntersectionTriangle(ray, *it);
		if (curIntersect.isHit && curIntersect.distanceHit < closestIntersect.distanceHit) {
			closestIntersect = curIntersect;
		}
	}
	return closestIntersect;
}
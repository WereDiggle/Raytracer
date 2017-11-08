#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"
#include "Ray.hpp"
#include "Intersect.hpp"

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}

};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );

  Intersect checkIntersection(const Ray & ray);
  
private:
	Intersect checkIntersectionTriangle(const Ray & ray, const Triangle & triangle);

	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;

	glm::vec3 boundingBoxMin;
	glm::vec3 boundingBoxMax;

	friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
	std::string assetFolder = "Assets";
};

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Intersect.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	virtual Intersect checkIntersection( const Ray & ray );
    //Intersect castRay(const Ray & ray);

	Material *m_material;
	Primitive *m_primitive;
};

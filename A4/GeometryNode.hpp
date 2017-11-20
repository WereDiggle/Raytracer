#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Ray.hpp"
#include "Intersect.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr , Texture *bitmap = Texture::defaultTexture, Texture *bumpmap = Texture::defaultTexture,
		double reflectiveness = 0, double diffuse = 1, double transparency = 0, double refraction = 1);

	void setMaterial( Material *material );
	void setBitmap( Texture *bitmap );
	void setBumpmap( Texture *bumpmap );
	void setReflectiveness(double reflectiveness);
	void setTransparency(double transparency, double refraction);
	void setDiffuse(double diffuse);

	virtual Intersect checkIntersection( const Ray & ray );

	Material *m_material;
	Texture *m_bitmap;
	Texture *m_bumpmap;
	Primitive *m_primitive;

	// reflectiveness + transparency + diffuse = 1
	double m_reflectiveness;
	double m_diffuse;
	double m_transparency;

	double m_refraction;
};

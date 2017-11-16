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
		Material *mat = nullptr , Texture *bitmap = Texture::defaultTexture, Texture *bumpmap = Texture::defaultTexture);

	void setMaterial( Material *material );
	void setBitmap( Texture *bitmap );
	void setBumpmap( Texture *bumpmap );

	virtual Intersect checkIntersection( const Ray & ray );

	Material *m_material;
	Texture *m_bitmap;
	Texture *m_bumpmap;
	Primitive *m_primitive;
};

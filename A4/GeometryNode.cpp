#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat, Texture *bitmap, Texture *bumpmap )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
	, m_bitmap( bitmap )
	, m_bumpmap( bumpmap )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

void GeometryNode::setBitmap( Texture * bitmap ) {
	m_bitmap = bitmap;
}

void GeometryNode::setBumpmap( Texture * bumpmap ) {
	m_bumpmap = bumpmap;
}

Intersect GeometryNode::checkIntersection( const Ray & ray )
{
	Intersect intersect = m_primitive->checkIntersection(ray);
	intersect.material = m_material;
	intersect.bitmap = m_bitmap;
	intersect.bumpmap = m_bumpmap;

	// Modify the normal Hit with the bumpmap
	if (intersect.isHit) {
		//std::cout << "old normal: " << glm::to_string(intersect.normalHit);
		intersect.normalHit = m_bumpmap->getNormal(intersect.normalHit, m_primitive->getUpV(intersect.textureU, intersect.textureU), intersect.textureU, intersect.textureV);
		//std::cout << ", new normal: " << glm::to_string(intersect.normalHit) << std::endl;
	}

	return intersect;
}